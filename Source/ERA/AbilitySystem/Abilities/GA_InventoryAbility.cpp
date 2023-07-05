// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_InventoryAbility.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "ERAGameTypes.h"
#include "DataAssets/FactionDataAsset/Faction.h"
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


	// Get the equipped weapon's static data
	const UWeaponItemStaticData* WeaponStaticData = GetEquippedWeaponItemStaticData();
	if (!WeaponStaticData)
	{
		return;
	}

	// Get the character
	const AERACharacter* Character = const_cast<AERACharacter*>(Cast<AERACharacter>(GetAvatarActorFromActorInfo()));
	if (!Character)
	{
		return;
	}

	// Detect nearby enemies
	TArray<AActor*> OverlappingActors;
	Character->GetEnemyDetectionSphere()->GetOverlappingActors(OverlappingActors);

	// Filter the overlapping actors to only include enemies
	TArray<AActor*> Enemies;
	for (AActor* Actor : OverlappingActors)
	{
		AERACharacter* PotentialEnemy = Cast<AERACharacter>(Actor);
		if (Character->Faction->Enemies.Contains(PotentialEnemy->Faction))
		{
			Enemies.Add(Actor);
		}
	}

	if (Enemies.Num() == 0 || Enemies.Num() == 1)
	{
		PlayNextMontage();
	}

	// // Choose a combo based on the number of enemies and the current combo in the string
	// UAnimMontage* MontageToPlay = nullptr;
	// if (Enemies.Num() == 1)
	// {
	// 	// Choose a single-target combo
	// 	MontageToPlay = WeaponStaticData->ComboAttacks[CurrentCombo];
	// }
	// else if (Enemies.Num() > 1)
	// {
	// 	// Choose a multi-target combo
	// 	MontageToPlay = WeaponStaticData->ComboAttacks[CurrentCombo];
	// }
	//
	// // Execute the combo
	// if (MontageToPlay)
	// {
	// 	// Play the montage...
	//
	// 	// Apply the attack to each target
	// 	for (AActor* Target : Enemies)
	// 	{
	// 		// Apply the attack to the target...
	// 	}
	//
	// 	// Increment the current combo
	// 	CurrentCombo++;
	//
	// 	// If we've reached the end of the combo string, play the finisher and reset the combo
	// 	if (CurrentCombo >= WeaponStaticData->ComboAttacks.Num())
	// 	{
	// 		MontageToPlay = WeaponStaticData->ComboFinisher;
	// 		CurrentCombo = 0;
	// 	}
	// }

	// Start a melee attack (OLD CODE)
			// if (UAnimMontage* MontageToPlay = GetEquippedItemMontage())
			// {
			// 	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			// 	{
			// 		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			// 		if (AnimInstance)
			// 		{
			// 			AnimInstance->Montage_Play(MontageToPlay);
			// 			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
			//
			// 			MontageTask->OnBlendOut.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
			// 			MontageTask->OnCompleted.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
			// 			MontageTask->OnInterrupted.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
			// 			MontageTask->OnCancelled.AddDynamic(this, &UGA_InventoryAbility::K2_EndAbility);
			// 			MontageTask->ReadyForActivation();
			// 		}
			// 	}
			// }
}

void UGA_InventoryAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	// Cancel the combo timer
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_InventoryAbility::PlayNextMontage()
{
	// Get the equipped weapon's static data
	const UWeaponItemStaticData* WeaponStaticData = GetEquippedWeaponItemStaticData();
	if (!WeaponStaticData)
	{
		return;
	}

	// Get the character
	const AERACharacter* Character = const_cast<AERACharacter*>(Cast<AERACharacter>(GetAvatarActorFromActorInfo()));
	if (!Character)
	{
		return;
	}
	
	// Check that the WeaponStaticData and its ComboAttacks array are not null and not empty
	if (!WeaponStaticData || WeaponStaticData->ComboAttacks.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponStaticData is null or ComboAttacks array is empty"));
		return;
	}

	// Check that CurrentCombo is within the bounds of the ComboAttacks array
	if (CurrentCombo < 0 || CurrentCombo >= WeaponStaticData->ComboAttacks.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentCombo is out of bounds"));
		return;
	}

	UAnimMontage* Montage = WeaponStaticData->ComboAttacks[CurrentCombo];
    
	// Check that the Character and its AnimInstance are not null
	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetAnimInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("Character or AnimInstance is null"));
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	float MontageLength = AnimInstance->Montage_Play(Montage);

	// Increment the current combo
	CurrentCombo++;

	// If we've reached the end of the combo string, reset the combo
	if (CurrentCombo >= WeaponStaticData->ComboAttacks.Num())
	{
		CurrentCombo = 0;
	}

	// Set a timer to call this function again after the montage finishes
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UGA_InventoryAbility::PlayNextMontage, MontageLength, false);
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
