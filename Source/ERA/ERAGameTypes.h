// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERAGameTypes.generated.h"

class AItemActor;
class UGameplayAbility;
class UGameplayEffect;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	class UCharacterAnimDataAsset* CharacterAnimDataAsset;
	
};

USTRUCT()
struct FCharacterAnimationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UBlendSpace* MovementBlendspace = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimSequenceBase* IdleAnimationAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UBlendSpace* CrouchMovementBlendspace = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimSequenceBase* CrouchIdleAnimationAsset = nullptr;
	
};

UENUM()
enum class EFoot : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};


USTRUCT(BlueprintType)
struct FMotionWarpingTargetByLocationAndRotation
{
	GENERATED_USTRUCT_BODY();

	FMotionWarpingTargetByLocationAndRotation()
	{

	}

	FMotionWarpingTargetByLocationAndRotation(FName InName, FVector InLocation, FQuat InRotation)
		: Name(InName)
		, Location(InLocation)
		, Rotation(InRotation)
	{

	}

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FQuat Rotation;
};

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<AItemActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName AttachmentSocket = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bCanBeThrown = false;
	
	UPROPERTY(EditDefaultsOnly)
	FCharacterAnimationData CharacterAnimationData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnGoingEffects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffects;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fists, // Melee
	Sword, // Melee
	Spear, // Melee and Ranged
	Axe, // Melee
	Shield, // Melee
	Dagger, // Melee
	Staff, // Melee
	Heavy_Blunt, // Melee
	Heavy_Bladed, // Melee
	Bow // Ranged
 };

UCLASS(BlueprintType, Blueprintable)
class UWeaponItemStaticData : public UItemStaticData
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Type")
	EWeaponType WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EffectsApplied")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponDamage")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ComboAnimMontages")
	TArray<UAnimMontage*> ComboAttacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ComboAnimMontages")
	UAnimMontage* ComboFinisher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* AttackSound;
};

UENUM()
enum class EItemState : uint8
{
	None UMETA(DisplayName = "None"),
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped UMETA(DisplayName = "Dropped")
};

UENUM()
enum class EMovementDirectionType : uint8
{
	None UMETA(DisplayName = "None"),
	OrientToMovement UMETA(DisplayName = "OrientToMovement"),
	StrafeMovement UMETA(DisplayName = "Strafe")
};
