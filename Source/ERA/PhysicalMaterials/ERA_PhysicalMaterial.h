// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ERA_PhysicalMaterial.generated.h"

class USoundBase;
class UNiagaraSystem;

UCLASS()
class ERA_API UERA_PhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalMaterial")
		USoundBase* FootstepSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalMaterial")
		USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalMaterial")
		UNiagaraSystem* ImpactVFX = nullptr;
};
