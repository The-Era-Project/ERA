// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERAGameTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ERAStatics.generated.h"

/**
 * 
 */
UCLASS()
class ERA_API UERAStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Static Data")
	static const UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass);
};
