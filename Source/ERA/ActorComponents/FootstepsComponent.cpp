// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/FootstepsComponent.h"
#include "PhysicalMaterials/ERA_PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "ERACharacter.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Show footstep debug lines")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

// Sets default values for this component's properties
UFootstepsComponent::UFootstepsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFootstepsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UFootstepsComponent::HandleFootstep(EFoot Foot)
{
	if (AERACharacter* Character = Cast<AERACharacter>(GetOwner()))
	{

		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnGameThread();
		
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				FHitResult HitResult;
				const FVector SocketLocation = Mesh->GetSocketLocation(Foot == EFoot::Left ? "LeftFootSocket" : "RightFootSocket");
				const FVector Location = SocketLocation + FVector::UpVector * 20.f;

				FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.bReturnPhysicalMaterial = true;
				CollisionQueryParams.AddIgnoredActor(Character);

				if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + FVector::UpVector * -50.f, ECollisionChannel::ECC_WorldStatic, CollisionQueryParams))
				{
					if (HitResult.bBlockingHit)
					{
						if (HitResult.PhysMaterial.Get())
						{
							UERA_PhysicalMaterial* PhysicalMaterial = Cast<UERA_PhysicalMaterial>(HitResult.PhysMaterial.Get());

							if (PhysicalMaterial)
							{
								UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->FootstepSound, Location, 1.f);
							}
							if (DebugShowFootsteps > 0)
							{
								DrawDebugString(GetWorld(), Location, GetNameSafe(PhysicalMaterial), nullptr, FColor::White, 4.f);
							}
							if (DebugShowFootsteps > 0)
							{
								DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, 4.f);
							}
						}
						else
						{
							if (DebugShowFootsteps > 0)
							{
								DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, 4.f, 0, 1);
							}
						}
					}
				}
				else
				{
					if (DebugShowFootsteps > 0)
					{
						DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, 4.f, 0, 1);
						DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, 4.f);
					}
				}
			}
		}
	}
}


