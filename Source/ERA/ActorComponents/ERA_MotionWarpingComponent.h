// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "ERAGameTypes.h"
#include "ERA_MotionWarpingComponent.generated.h"


UCLASS()
class ERA_API UERA_MotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()

public:

	UERA_MotionWarpingComponent(const FObjectInitializer& ObjectInitializer);

	// Depricated 5.1
	
	/*
	void SendWarpPointsToClients();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncWarpPoints(const TArray<FMotionWarpingTargetByLocationAndRotation>& Targets);
	*/

	
};
