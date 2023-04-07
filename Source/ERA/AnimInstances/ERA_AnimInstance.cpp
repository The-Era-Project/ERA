// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/ERA_AnimInstance.h"
#include "ERACharacter.h"
#include "ERAGameTypes.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimationAsset.h"
#include "Animation/BlendSpace.h"
#include "DataAssets/CharacterAnimDataAsset.h"



UBlendSpace* UERA_AnimInstance::GetLocomotionBlendSpace() const
{
	if (AERACharacter* ERACharacter = Cast<AERACharacter>(GetOwningActor()))
	{
		FCharacterData Data = ERACharacter->GetCharacterData();

		if (Data.CharacterAnimDataAsset)
		{
			return Data.CharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace;
		}
	}
	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace : nullptr;
}

UAnimSequenceBase* UERA_AnimInstance::GetIdleAnimation() const
{
	if (AERACharacter* ERACharacter = Cast<AERACharacter>(GetOwningActor()))
	{
		FCharacterData Data = ERACharacter->GetCharacterData();

		if (Data.CharacterAnimDataAsset)
		{
			return Data.CharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset;
		}
	}
	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
}
