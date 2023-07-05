// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemInterface.h"
#include "ERAGameTypes.h"
#include "ActorComponents/ERA_MotionWarpingComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "DataAssets/FactionDataAsset/Faction.h"
#include "ERACharacter.generated.h"



class UERA_AbilitySystemComponentBase;
class UERA_AttributeSetBase;

class UGameplayAbility;
class UGameplayEffect;

class ERA_MotionWarpingComponent;
class UERA_CharacterMovementComponent;

class UInventoryComponent;

UCLASS(config=Game)
class AERACharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// Enhanced Input System

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchInputAction;

	/* Sprint Input Action */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* SprintInputAction;

	/* Drop Item Action */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* DropItemInputAction;

	/* Equip Next Item Action */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipNextInputAction;

	/* Unequip Item Action */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* UnequipInputAction;

	/* Attack */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* AttackInputAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	void OnJumpActionStarted(const FInputActionValue& Value);

	void OnJumpActionEnded(const FInputActionValue& Value);

	void OnCrouchActionStarted(const FInputActionValue& Value);

	void OnCrouchActionEnded(const FInputActionValue& Value);

	void OnSprintActionStarted(const FInputActionValue& Value);

	void OnSprintActionEnded(const FInputActionValue& Value);

	void OnDropItemTriggered(const FInputActionValue& Value);

	void OnEquipNextTriggered(const FInputActionValue& Value);

	void OnUnequipTriggered(const FInputActionValue& Value);

	void OnAttackActionStarted(const FInputActionValue& Value);

	void OnAttackActionEnded(const FInputActionValue& Value);
	

public:
	AERACharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext);

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	UERA_MotionWarpingComponent* GetERAMotionWarpingComponent() const;

	UInventoryComponent* GetInventoryComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	UFaction* Faction;
	

protected:

	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly)
	UERA_AbilitySystemComponentBase* AbilitySystemComponent;
	
	UPROPERTY(Transient)
	UERA_AttributeSetBase* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarp)
	UERA_MotionWarpingComponent* ERAMotionWarpingComponent;

	UERA_CharacterMovementComponent* ERACharacterMovementComponent;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	bool bIsAiming = false;

	// Add helper functions to work wit the data assets

	UFUNCTION(BlueprintCallable)
	FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FCharacterData& InCharacterData);

	class UFootstepsComponent* GetFootstepsComponent() const;

	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
	
	USphereComponent* GetEnemyDetectionSphere() const { return EnemyDetectionSphere; }

protected:

	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();

	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	class UCharacterDataAsset* CharacterDataAsset;

	UPROPERTY(BlueprintReadOnly)
	class UFootstepsComponent* FootstepsComponent;

	// Collision shape for detecting enemies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* EnemyDetectionSphere;

	// Gameplay Events

protected:
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackStartedEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackEndedEventTag;

	// Gameplay Tags
protected:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CrouchTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SprintTags;

	// Gameplay Effects

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CrouchStateEffect;

	// Delegates
	FDelegateHandle MaxMovementSpeedChangedDelegateHandle;

	// Inventory
	UPROPERTY(EditAnywhere, Replicated)
	UInventoryComponent* InventoryComponent = nullptr;
	
};

