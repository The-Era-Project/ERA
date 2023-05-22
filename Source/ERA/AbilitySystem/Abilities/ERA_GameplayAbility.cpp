// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ERA_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "ERACharacter.h"
#include "AbilitySystemLog.h"

void UERA_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();

	for (auto GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect)
		{
			continue;
		}
		FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
			GameplayEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ActorInfo->AbilitySystemComponent->
			                                                        ApplyGameplayEffectSpecToSelf(
				                                                        *SpecHandle.Data.Get());
			if (!ActiveGEHandle.WasSuccessfullyApplied())
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to apply %s GameplayEffect! %s"), *GetNameSafe(GameplayEffect),
				       *GetNameSafe(ActorInfo->AvatarActor.Get()));
			}
		}
	}
	if (IsInstantiated())
	{
		for (auto GameplayEffect : OngoingEffectsToRemoveOnEnd)
		{
			if (!GameplayEffect)
			{
				continue;
			}
			if (UAbilitySystemComponent* AbilityComponent = ActorInfo->AbilitySystemComponent.Get())
			{
				FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
					GameplayEffect, 1, EffectContext);
				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveGEHandle = ActorInfo->AbilitySystemComponent->
					                                                        ApplyGameplayEffectSpecToSelf(
						                                                        *SpecHandle.Data.Get());
					if (!ActiveGEHandle.WasSuccessfullyApplied())
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to apply %s runtime GameplayEffect! %s"),
						       *GetNameSafe(GameplayEffect), *GetNameSafe(ActorInfo->AvatarActor.Get()));
					}
					else
					{
						RemoveOnEndEffectHandles.Add(ActiveGEHandle);
					}
				}
			}
		}
	}
}

void UERA_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsInstantiated())
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : RemoveOnEndEffectHandles)
		{
			if (ActiveEffectHandle.IsValid())
			{
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}

		RemoveOnEndEffectHandles.Empty();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AERACharacter* UERA_GameplayAbility::GetERACharacterFromActorInfo() const
{
	return Cast<AERACharacter>(GetAvatarActorFromActorInfo());
}
