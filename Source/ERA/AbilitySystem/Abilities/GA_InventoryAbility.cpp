// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_InventoryAbility.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "ERAGameTypes.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Actors/ItemActor.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryItemInstance.h"
#include "ERACharacter.h"
#include "AnimInstances/ERA_AnimInstance.h"
#include "Inventory/WeaponItemActor.h"

class UERA_AnimInstance;

void UGA_InventoryAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
}

void UGA_InventoryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Start a melee attack
			if (UAnimMontage* MontageToPlay = GetEquippedItemMontage())
			{
				if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
				{
					UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
					if (AnimInstance)
					{
						AnimInstance->Montage_Play(MontageToPlay);
						MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
	
						MontageTask->OnBlendOut.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
						MontageTask->OnCompleted.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
						MontageTask->OnInterrupted.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
						MontageTask->OnCancelled.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
						MontageTask->ReadyForActivation();
					}
				}
			}
}

void UGA_InventoryAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UInventoryComponent* UGA_InventoryAbility::GetInventoryComponent() const
{
	return InventoryComponent;
}

UInventoryItemInstance* UGA_InventoryAbility::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

const UItemStaticData* UGA_InventoryAbility::GetEquippedItemStaticData() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemStaticData() : nullptr;
}

const UWeaponItemStaticData* UGA_InventoryAbility::GetEquippedWeaponItemStaticData() const
{
	// Get the equipped item instance
	if (const UInventoryItemInstance* EquippedWeaponItem = GetEquippedItemInstance())
	{
		return Cast<UWeaponItemStaticData>(EquippedWeaponItem->GetWeaponItemStaticData());
	}
	return nullptr;
}

AItemActor* UGA_InventoryAbility::GetEquippedItemActor() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemActor() : nullptr;
}

AWeaponItemActor* UGA_InventoryAbility::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}

UAnimMontage* UGA_InventoryAbility::GetEquippedItemMontage() const
{
	if(const UWeaponItemStaticData* WeaponStaticData = GetEquippedWeaponItemStaticData())
	{
		if(UAnimMontage* Montage = WeaponStaticData->AttackAnimMontage)
		{
			return Montage;
		}
	}

	return nullptr;
}
