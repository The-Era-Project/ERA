// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ERAGameTypes.h"
#include "GameplayTagContainer.h"
#include "ERA_CharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class ERA_API UERA_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	bool TryTraversal(UAbilitySystemComponent* ASC);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	EMovementDirectionType GetMovementDirectionType() const;

	UFUNCTION(BlueprintCallable)
	void SetMovementDirectionType(EMovementDirectionType InMovementDirectionType);

	UFUNCTION()
	void OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;

	UPROPERTY(EditAnywhere)
	EMovementDirectionType MovementDirectionType;
	
	void HandleMovementDirection();
	
};
