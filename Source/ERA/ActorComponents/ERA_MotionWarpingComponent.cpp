// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ERA_MotionWarpingComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystemComponent.h"

UERA_MotionWarpingComponent::UERA_MotionWarpingComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}


// Depricated 5.1
/*
void UERA_MotionWarpingComponent::SendWarpPointsToClients()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		TArray<FMotionWarpingTargetByLocationAndRotation> WarpTargets;

		for (auto WarpTarget : WarpTargetMap)
		{
			FMotionWarpingTargetByLocationAndRotation MotionWarpingTarget(WarpTarget.Key, WarpTarget.Value.GetLocation(), WarpTarget.Value.GetRotation());

			WarpTargets.Add(MotionWarpingTarget);
		}

		MulticastSyncWarpPoints(WarpTargets);
	}
}

void UERA_MotionWarpingComponent::MulticastSyncWarpPoints_Implementation(const TArray<FMotionWarpingTargetByLocationAndRotation>& Targets)
{
	for (const FMotionWarpingTargetByLocationAndRotation& Target : Targets)
	{
		AddOrUpdateWarpTargetFromLocationAndRotation(Target.Name, Target.Location, FRotator(Target.Rotation));
	}
}
*/
