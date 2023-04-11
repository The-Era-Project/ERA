// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ERA_CharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class ERA_API UERA_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	bool TryTraversal(UAbilitySystemComponent* ASC);

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;
	
};
