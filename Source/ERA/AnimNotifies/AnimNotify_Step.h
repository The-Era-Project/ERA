// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ERAGameTypes.h"
#include "AnimNotify_Step.generated.h"

/**
 * 
 */
UCLASS()
class ERA_API UAnimNotify_Step : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	EFoot Foot;
	
};
