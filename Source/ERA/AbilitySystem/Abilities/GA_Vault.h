// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ERA_GameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "GA_Vault.generated.h"

class UAbilityTask_PlayMontageAndWait;


UCLASS()
class ERA_API UGA_Vault : public UERA_GameplayAbility
{
	GENERATED_BODY()

	UGA_Vault();

	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/*UFUNCTION()
	void HandleVaultTimelineUpdate(float Value);*/

	UPROPERTY()
	UTimelineComponent* VaultTimeline;

	// Add our Horizontal traces

	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float HorizontalTraceRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float HorizontalTraceLength = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float HorizontalTraceCount = 5.f;

	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float HorizontalTraceStep = 30.f;

	// Add our vertical traces

	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float VerticalTraceRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category= HorizontalTrace)
	float VerticalTraceStep = 30.f;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* VaultMontage = nullptr;

	UAbilityTask_PlayMontageAndWait* MontageTask = nullptr;

	FVector JumpToLocation;
	FVector JumpOverLocation;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToIgnore;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vault")
	UCurveFloat* VaultCurve;*/
};