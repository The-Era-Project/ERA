#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
// #include "DataAssets/FactionDataAsset/Faction.h"
#include "DataAssets/Resource/ResourceDataAsset.h"
#include "DataAssets/Resource/ResourceInstance.h"
#include "DataAssets/Building/BuildingDataAsset.h"
#include "Cities.generated.h"

class UFaction;

USTRUCT(BlueprintType)
struct FBuildingInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UBuildingDataAsset* Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float ConstructionTimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	bool bIsUnderConstruction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	bool bIsDamaged;
};

UCLASS()
class ERA_API UCity : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "City")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "City")
	UFaction* OwningFaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "City")
	TArray<FResourceInstance> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "City")
	TArray<FBuildingInstance> Buildings;

	// Add a resource to the city's resources
	void AddResource(FResourceInstance NewResource);

	// Produce a resource, increasing its quantity
	void ProduceResource(UResourceDataAsset* Resource, int32 Quantity);

	// Consume a resource, decreasing its quantity
	void ConsumeResource(UResourceDataAsset* Resource, int32 Quantity);

	// Add a building to the city's buildings
	void AddBuilding(FBuildingInstance NewBuilding);

	// Start construction of a building
	void StartConstruction(UBuildingDataAsset* Building);

	// Update construction progress of all buildings
	void UpdateConstruction(float DeltaTime);

	// Finish construction of a building
	void FinishConstruction(UBuildingDataAsset* Building);

	// Repair a damaged building
	void RepairBuilding(UBuildingDataAsset* Building);
};

