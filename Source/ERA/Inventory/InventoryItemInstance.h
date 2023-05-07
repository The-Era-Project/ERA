// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERAGameTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
//#inlcude "GameplayAbilitySpec.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ERA_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable)
	const UItemStaticData* GetItemStaticData() const;

	UFUNCTION(BlueprintCallable)
	const UWeaponItemStaticData* GetWeaponItemStaticData() const;


	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;
	
	UPROPERTY(Replicated)
	TSubclassOf<UWeaponItemStaticData> WeaponItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool bEquipped = false;

	UFUNCTION()
	void OnRep_Equipped();

	virtual void OnEquipped(AActor* InOwner = nullptr);
	virtual void OnUnequipped(AActor* InOwner = nullptr);
	virtual void OnDropped(AActor* InOwner = nullptr);
	
	UFUNCTION(BlueprintPure)
	AItemActor* GetItemActor() const;

protected:

	UPROPERTY(Replicated)
	AItemActor* ItemActor = nullptr;

	void TryGrantAbilities(AActor* InOwner);

	void TryRemoveAbilities(AActor* InOwner);

	void TryApplyEffects(AActor* InOwner);

	void TryRemoveEffects(AActor* InOwner);

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	TArray<FActiveGameplayEffectHandle> OngoingEffectHandles;
};
