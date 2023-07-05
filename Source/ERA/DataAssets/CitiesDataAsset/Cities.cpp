// Fill out your copyright notice in the Description page of Project Settings.


#include "Cities.h"


void UCity::AddResource(FResourceInstance NewResource)
{
    // Check if the resource already exists in the city's resources
    for (const auto& Res : Resources)
    {
        if (Res.Resource == NewResource.Resource)
        {
            UE_LOG(LogTemp, Warning, TEXT("Resource already exists in the city's resources."));
            return;
        }
    }

    // Add the new resource to the city's resources
    Resources.Add(NewResource);
}

void UCity::ProduceResource(UResourceDataAsset* Resource, int32 Quantity)
{
    // Find the resource in the city's resources and increase its quantity
    for (auto& Res : Resources)
    {
        if (Res.Resource == Resource)
        {
            Res.Quantity += Quantity;
            break;
        }
    }
}

void UCity::ConsumeResource(UResourceDataAsset* Resource, int32 Quantity)
{
    // Find the resource in the city's resources and decrease its quantity
    for (auto& Res : Resources)
    {
        if (Res.Resource == Resource)
        {
            Res.Quantity -= Quantity;
            break;
        }
    }
}

void UCity::AddBuilding(FBuildingInstance NewBuilding)
{
    // Add the new building to the city's buildings
    Buildings.Add(NewBuilding);
}

void UCity::StartConstruction(UBuildingDataAsset* Building)
{
    // Find the building in the city's buildings and start its construction
    for (auto& Bld : Buildings)
    {
        if (Bld.Building == Building)
        {
            Bld.bIsUnderConstruction = true;
            break;
        }
    }
}

void UCity::UpdateConstruction(float DeltaTime)
{
    // Update the construction progress of all buildings
    for (auto& Bld : Buildings)
    {
        if (Bld.bIsUnderConstruction)
        {
            Bld.ConstructionTimeRemaining -= DeltaTime;
            if (Bld.ConstructionTimeRemaining <= 0)
            {
                FinishConstruction(Bld.Building);
            }
        }
    }
}

void UCity::FinishConstruction(UBuildingDataAsset* Building)
{
    // Find the building in the city's buildings and finish its construction
    for (auto& Bld : Buildings)
    {
        if (Bld.Building == Building)
        {
            Bld.bIsUnderConstruction = false;
            Bld.ConstructionTimeRemaining = 0;
            break;
        }
    }
}

void UCity::RepairBuilding(UBuildingDataAsset* Building)
{
    // Find the building in the city's buildings and repair it
    for (auto& Bld : Buildings)
    {
        if (Bld.Building == Building)
        {
            Bld.bIsDamaged = false;
            break;
        }
    }
}