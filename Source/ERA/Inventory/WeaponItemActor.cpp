// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/WeaponItemActor.h"
#include "Inventory/InventoryItemInstance.h"
#include "ERAGameTypes.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/ERA_PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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

void AWeaponItemActor::PlayWeaponEffects(const FHitResult& InHitResult)
{
	if(HasAuthority())
	{
		MulticastPlayWeaponEffects(InHitResult);
	}
	else
	{
		PlayWeaponEffects_Internal(InHitResult);
	}
}

void AWeaponItemActor::MulticastPlayWeaponEffects_Implementation(const FHitResult& InHitResult)
{
	if (!Owner || Owner->GetLocalRole() != ROLE_AutonomousProxy)
	{
		PlayWeaponEffects_Internal(InHitResult);
	}
}

void AWeaponItemActor::PlayWeaponEffects_Internal(const FHitResult& InHitResult)
{
	if (InHitResult.PhysMaterial.Get())
	{
		UERA_PhysicalMaterial* PhysicalMaterial = Cast<UERA_PhysicalMaterial>(InHitResult.PhysMaterial.Get());

		if (PhysicalMaterial)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->ImpactSound, InHitResult.ImpactPoint, 1.f);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PhysicalMaterial->ImpactVFX, InHitResult.ImpactPoint, InHitResult.ImpactNormal.Rotation());
		}

		if (const UWeaponItemStaticData* WeaponData = GetWeaponItemStaticData())
		{
			if (WeaponData->AttackSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, WeaponData->AttackSound, GetActorLocation(), 1.f);
			}
		}
	}
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
