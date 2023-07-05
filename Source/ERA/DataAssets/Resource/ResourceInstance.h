#pragma once

#include "DataAssets/Resource/ResourceDataAsset.h"
#include "CoreMinimal.h"
#include "ResourceInstance.generated.h"

USTRUCT(BlueprintType)
struct FResourceInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	UResourceDataAsset* Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Quantity;
};
