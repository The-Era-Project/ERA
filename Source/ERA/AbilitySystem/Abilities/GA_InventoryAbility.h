// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERAGameTypes.h"
#include "AbilitySystem/Abilities/ERA_GameplayAbility.h"
#include "Inventory/WeaponItemActor.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GA_InventoryAbility.generated.h"

class UInventoryComponent;
class UInventoryItemInstance;
class UWeaponItemStaticData;
class AItemActor;

UCLASS()
class ERA_API UGA_InventoryAbility : public UERA_GameplayAbility
{
	GENERATED_BODY()

public:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void PlayNextMontage();
	
	UFUNCTION(Blueprintpure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(Blueprintpure)
	UInventoryItemInstance* GetEquippedItemInstance() const;

	UFUNCTION(Blueprintpure)
	const UItemStaticData* GetEquippedItemStaticData() const;

	UFUNCTION(Blueprintpure)
	const UWeaponItemStaticData* GetEquippedWeaponItemStaticData() const;

	UFUNCTION(Blueprintpure)
	AItemActor* GetEquippedItemActor() const;

	UFUNCTION(Blueprintpure)
	AWeaponItemActor* GetEquippedWeaponItemActor() const;

	UFUNCTION(BlueprintPure)
	UAnimMontage* GetEquippedItemMontage() const;

protected:
	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask = nullptr;
	
	FTimerHandle ComboTimerHandle;

	// Current Combo
	UPROPERTY(Transient)
	int32 CurrentCombo = 0;
	
};
