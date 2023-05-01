// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ERA_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


static TAutoConsoleVariable<int32> CVarShowTraversal(
	TEXT("ShowDebugTraversal"),
	0,
	TEXT("Draws debug info about traversal")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

void UERA_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	HandleMovementDirection();

	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		AbilityComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("Movement.Enforced.Strafe")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UERA_CharacterMovementComponent::OnEnforcedStrafeTagChanged);
	}
}

bool UERA_CharacterMovementComponent::TryTraversal(UAbilitySystemComponent* ASC)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : TraversalAbilitiesOrdered)
	{
		if (ASC->TryActivateAbilityByClass(AbilityClass, true))
		{
			FGameplayAbilitySpec* Spec;

			Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
			if (Spec && Spec->IsActive())
			{
				return true;
			}
		}
	}

	return false;
}

EMovementDirectionType UERA_CharacterMovementComponent::GetMovementDirectionType() const
{
	return MovementDirectionType;
}

void UERA_CharacterMovementComponent::SetMovementDirectionType(EMovementDirectionType InMovementDirectionType)
{
	MovementDirectionType = InMovementDirectionType;
	HandleMovementDirection();
}

void UERA_CharacterMovementComponent::OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount)
	{
		SetMovementDirectionType(EMovementDirectionType::StrafeMovement);
	}
	else
	{
		SetMovementDirectionType(EMovementDirectionType::OrientToMovement);
	}
}

void UERA_CharacterMovementComponent::HandleMovementDirection()
{
	switch (MovementDirectionType)
	{
	case EMovementDirectionType::StrafeMovement:
		bUseControllerDesiredRotation = true;
		bOrientRotationToMovement = false;
		CharacterOwner->bUseControllerRotationYaw = true;
		break;
	default:
		bUseControllerDesiredRotation = false;
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = false;
		break;
	}
}
