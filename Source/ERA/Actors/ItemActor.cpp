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
	// MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// MeshComponent->SetupAttachment(RootComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnSphereOverlap);
}

void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;

	InitInternal();
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

		const float droppItemDist = 100.f;
		const float droppItemTraceDist = 10000.f;

		const FVector TraceStart = Location + Forward * droppItemDist;
		const FVector TraceEnd = TraceStart - FVector::UpVector * droppItemTraceDist;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };

		FHitResult TraceHit;

		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;

		FVector TargetLocation = TraceEnd;

		EDrawDebugTrace::Type DebugDrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, DebugDrawType, TraceHit, true))
		{
			if (TraceHit.bBlockingHit)
			{
				TargetLocation = TraceHit.Location;
			}
		}

		SetActorLocation(TargetLocation);

		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
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

			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);

			InitInternal();
		}
	}
}

void AItemActor::OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance)
{
	if (IsValid(ItemInstance) && !IsValid(OldItemInstance))
	{
		InitInternal();
	}
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

void AItemActor::InitInternal()
{
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

