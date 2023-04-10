// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ERA_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ERA_API UERA_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

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
	
};
