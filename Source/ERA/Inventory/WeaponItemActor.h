// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemActor.h"
#include "WeaponItemActor.generated.h"

/**
 * 
 */
UCLASS()
class ERA_API AWeaponItemActor : public AItemActor
{
	GENERATED_BODY()

	AWeaponItemActor();

	const UWeaponItemStaticData* GetWeaponItemStaticData() const;

	UFUNCTION(BlueprintPure)
	FVector GetProjectileSpawnLocation() const;

	UFUNCTION(BlueprintCallable)
	void PlayWeaponEffects(const FHitResult& InHitResult);

protected:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponEffects(const FHitResult& InHitResult);

	void PlayWeaponEffects_Internal(const FHitResult& InHitResult);

	virtual void InitInternal() override;
	
};
