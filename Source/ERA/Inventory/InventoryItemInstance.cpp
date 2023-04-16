// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"
#include "GameFramework/Character.h"
#include "ERAStatics.h"
#include "Actors/ItemActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UERAStatics::GetItemStaticData(ItemStaticDataClass);
}

void UInventoryItemInstance::OnRep_Equipped()
{
}

void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	if (UWorld* World = InOwner->GetWorld())
	{
		const UItemStaticData* StaticData = GetItemStaticData();

		FTransform Transform;
		ItemActor =	World->SpawnActorDeferred<AItemActor>(StaticData->ItemActorClass, Transform, InOwner);
		ItemActor->Init(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(Transform);

		ACharacter* Character = Cast<ACharacter>(InOwner);
		if (USkeletalMeshComponent* SkeletalMesh = Character ? Character->GetMesh() : nullptr)
		{
			ItemActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, StaticData->AttachmentSocket);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("ItemActor attached to socket: %s"), *StaticData->AttachmentSocket.ToString()));
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("ItemActor location after attachment: %s"), *ItemActor->GetActorLocation().ToString()));
			}
		}
	}
	bEquipped = true;
}

void UInventoryItemInstance::OnUnequipped()
{
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
	bEquipped = false;
}

void UInventoryItemInstance::OnDropped()
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
	}
	bEquipped = false;
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UInventoryItemInstance, bEquipped);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
}
