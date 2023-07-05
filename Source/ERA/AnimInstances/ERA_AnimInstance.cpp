// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/ERA_AnimInstance.h"
#include "ERACharacter.h"
#include "ERAGameTypes.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimationAsset.h"
#include "Animation/BlendSpace.h"
#include "DataAssets/CharacterDataAsset/CharacterAnimDataAsset.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "GameFramework/CharacterMovementComponent.h"



void UERA_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ERACharacter = Cast<AERACharacter>(TryGetPawnOwner());
}

void UERA_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ERACharacter == nullptr)
	{
		ERACharacter = Cast<AERACharacter>(TryGetPawnOwner());
	}
	if (ERACharacter == nullptr)
	{
		return;
	}
	FVector Velocity = ERACharacter->GetVelocity();
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();
	bIsInAir = ERACharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ERACharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}

const UItemStaticData* UERA_AnimInstance::GetEquippedItemData() const
{
	//AERACharacter* ERACharacter = Cast<AERACharacter>(GetOwningActor());
	UInventoryComponent* InventoryComponent = ERACharacter ? ERACharacter->GetInventoryComponent() : nullptr;
	UInventoryItemInstance* ItemInstance = InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
	return ItemInstance ? ItemInstance->GetItemStaticData() : nullptr;
}

const UItemStaticData* UERA_AnimInstance::GetEquippedWeaponItemData() const
{
	//AERACharacter* ERACharacter = Cast<AERACharacter>(GetOwningActor());
	UInventoryComponent* InventoryComponent = ERACharacter ? ERACharacter->GetInventoryComponent() : nullptr;
	UInventoryItemInstance* ItemInstance = InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
	return ItemInstance ? ItemInstance->GetWeaponItemStaticData() : nullptr;
}

UBlendSpace* UERA_AnimInstance::GetLocomotionBlendSpace() const
{
	if (ERACharacter == nullptr)
	{
		return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace : nullptr;
	}
    
	if (const UItemStaticData* ItemData = GetEquippedItemData())
	{
		if (ItemData->CharacterAnimationData.MovementBlendspace)
		{
			return ItemData->CharacterAnimationData.MovementBlendspace;
		}
	}

	FCharacterData Data = ERACharacter->GetCharacterData();
	if (Data.CharacterAnimDataAsset)
	{
		return Data.CharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace;
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace : nullptr;
}

UAnimSequenceBase* UERA_AnimInstance::GetIdleAnimation() const
{
	if (ERACharacter == nullptr)
	{
		return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
	}
    
	if (const UItemStaticData* ItemData = GetEquippedItemData())
	{
		if (ItemData->CharacterAnimationData.IdleAnimationAsset)
		{
			return ItemData->CharacterAnimationData.IdleAnimationAsset;
		}
	}

	FCharacterData Data = ERACharacter->GetCharacterData();
	if (Data.CharacterAnimDataAsset)
	{
		return Data.CharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset;
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
}

UBlendSpace* UERA_AnimInstance::GetCrouchLocomotionBlendSpace() const
{
	if (ERACharacter == nullptr)
	{
		return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
	}
    
	if (const UItemStaticData* ItemData = GetEquippedItemData())
	{
		if (ItemData->CharacterAnimationData.CrouchMovementBlendspace)
		{
			return ItemData->CharacterAnimationData.CrouchMovementBlendspace;
		}
	}

	FCharacterData Data = ERACharacter->GetCharacterData();
	if (Data.CharacterAnimDataAsset)
	{
		return Data.CharacterAnimDataAsset->CharacterAnimationData.CrouchMovementBlendspace;
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
}

UAnimSequenceBase* UERA_AnimInstance::GetCrouchIdleAnimation() const
{
	if (ERACharacter == nullptr)
	{
		return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
	}
    
	if (const UItemStaticData* ItemData = GetEquippedItemData())
	{
		if (ItemData->CharacterAnimationData.CrouchIdleAnimationAsset)
		{
			return ItemData->CharacterAnimationData.CrouchIdleAnimationAsset;
		}
	}

	FCharacterData Data = ERACharacter->GetCharacterData();
	if (Data.CharacterAnimDataAsset)
	{
		return Data.CharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset;
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}
