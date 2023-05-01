// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/WeaponItemActor.h"
#include "Inventory/InventoryItemInstance.h"
#include "ERAGameTypes.h"

UStaticMeshComponent* StaticMeshComponent;
USkeletalMeshComponent* SkeletalMeshComponent;

AWeaponItemActor::AWeaponItemActor()
{

	// Create the components in constructor
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	// Initially, deactivate both components
	StaticMeshComponent->SetVisibility(false);
	SkeletalMeshComponent->SetVisibility(false);
}

const UWeaponItemStaticData* AWeaponItemActor::GetWeaponItemStaticData() const
{
	return ItemInstance ? Cast<UWeaponItemStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}

FVector AWeaponItemActor::GetProjectileSpawnLocation() const
{
	return MeshComponent ? MeshComponent->GetSocketLocation("ProjectileSpawnLoc") : GetActorLocation();
}

void AWeaponItemActor::InitInternal()
{
	Super::InitInternal();

	if (const UWeaponItemStaticData* WeaponData = GetWeaponItemStaticData())
	{
		if (WeaponData->SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMesh(WeaponData->SkeletalMesh);
			SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			SkeletalMeshComponent->SetVisibility(true);
			StaticMeshComponent->SetVisibility(false);
		}
		else if (WeaponData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(WeaponData->StaticMesh);
			StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			StaticMeshComponent->SetVisibility(true);
			SkeletalMeshComponent->SetVisibility(false);
		}
	}
}
