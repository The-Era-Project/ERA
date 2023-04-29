// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ItemActor.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ERACharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/InventoryItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	

	// Make sure the MeshComponent is created and attached
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnSphereOverlap);
}

void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

void AItemActor::OnEquipped()
{
	
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void AItemActor::OnUnequipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void AItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;
	
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = ActorOwner->GetActorForwardVector();

		// Add trace start and end
		
		constexpr float dropItemDistance = 100.f;
		constexpr float dropItemTraceDistance = 1000.f;
		
		const FVector TraceStart = Location + Forward * dropItemDistance;
		const FVector TraceEnd = TraceStart - FVector::UpVector * dropItemTraceDistance;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };
		FHitResult TraceHit;

		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;
		
		FVector TargetLocation = TraceEnd;
		EDrawDebugTrace::Type DrawDebugType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		if(UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, DrawDebugType, TraceHit, true))
		{
			if (TraceHit.bBlockingHit)
			{
				TargetLocation = TraceHit.Location;
			}
		}
		SetActorLocation(TargetLocation);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(false);
		
	}
}

bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		
	return WroteSomething;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass);
		}
	}
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetGenerateOverlapEvents(true);
}

void AItemActor::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::Equipped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	default:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
	}
}

/*
* The OnSphereOverlap function is executed when the item actor's sphere (collision component) overlaps with another actor's component in the game world.
* The function checks if the current item actor has authority (i.e., it is running on the server), and if it does,
* it sends a gameplay event to the overlapping actor (OtherActor) to inform it of the overlap with the item actor.
* The function sets up an FGameplayEventData structure, which contains information about the event,
* such as the instigator (the item actor itself), the optional object (the ItemInstance of the item actor),
* and an event tag (EquipItemActorTag).
* This information can be used by the recipient actor to determine the type of event and take appropriate actions,
* such as picking up the item or adding it to the character's inventory.
* The actual outcome of this function depends on how the recipient actor processes the gameplay event.
* If the recipient actor is set up to handle the event properly,
* it might result in the character picking up the item and updating its inventory.
* If not, there might be no visible effect in the game world.
 */

// Function to handle the sphere overlap event of the item actor
void AItemActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// Check if the current item actor has authority (e.g., it is running on the server)
	if (HasAuthority())
	{		
		// Create a new FGameplayEventData structure for the payload of the gameplay event
		FGameplayEventData EventPayload;

		// Set the instigator of the event as the current item actor
		EventPayload.Instigator = this;

		// Set the optional object of the event as the ItemInstance of the current item actor
		EventPayload.OptionalObject = ItemInstance;

		// Set the event tag as the EquipItemActorTag, which may be used to identify the event type
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;

		// Send the gameplay event with the EquipItemActorTag and the event payload to the overlapping actor (OtherActor)
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);
	}
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
	DOREPLIFETIME(AItemActor, ItemState);
}

