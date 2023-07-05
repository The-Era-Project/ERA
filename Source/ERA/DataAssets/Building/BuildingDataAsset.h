#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/Resource/ResourceDataAsset.h"
#include "BuildingDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ERA_API UBuildingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TArray<UResourceDataAsset*> ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float ConstructionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	TArray<UResourceDataAsset*> Production;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float ProductionRate;
};
