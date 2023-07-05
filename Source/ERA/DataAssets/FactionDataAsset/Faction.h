#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Resource/ResourceInstance.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Faction.generated.h"

class UCity;

UENUM(BlueprintType)
enum class ECulture : uint8
{
	Greek,
	Roman,
	Carthaginian,
	Gallic
};

UCLASS()
class ERA_API UFaction : public UDataAsset
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	ECulture Culture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	int32 StrengthIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
	TMap<UFaction*, int32> RelationshipScores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
	TArray<UFaction*> Allies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
	TArray<UFaction*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
	TArray<UFaction*> Neutrals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	TArray<FResourceInstance> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	UCity* CapitalCity;
};
