// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ERACharacter.h"
#include "ERA_AnimInstance.generated.h"


class UItemStaticData;

UCLASS()
class ERA_API UERA_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:

	const UItemStaticData* GetEquippedItemData() const;

	const UItemStaticData* GetEquippedWeaponItemData() const;

	UFUNCTION(BlueprintCallable,meta = (BlueprintThreadsafe), Category = "Animation")
	class UBlendSpace* GetLocomotionBlendSpace() const;

	UFUNCTION(BlueprintCallable,meta = (BlueprintThreadsafe), Category = "Animation")
	class UAnimSequenceBase* GetIdleAnimation() const;

	UFUNCTION(BlueprintCallable,meta = (BlueprintThreadsafe), Category = "Animation")
	class UBlendSpace* GetCrouchLocomotionBlendSpace() const;

	UFUNCTION(BlueprintCallable,meta = (BlueprintThreadsafe), Category = "Animation")
	class UAnimSequenceBase* GetCrouchIdleAnimation() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	class UCharacterAnimDataAsset* DefaultCharacterAnimDataAsset;

private:

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AERACharacter* ERACharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	
};
