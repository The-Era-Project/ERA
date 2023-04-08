// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_Step.h"
#include "ERACharacter.h"
#include "ActorComponents/FootstepsComponent.h"

void UAnimNotify_Step::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	Super::Notify(MeshComp, Animation);

	check(MeshComp);

	AERACharacter* Character = MeshComp ? Cast<AERACharacter>(MeshComp->GetOwner()) : nullptr;

	if (Character)
	{
		if (UFootstepsComponent* FootstepsComponent = Character->GetFootstepsComponent())
		{
			FootstepsComponent->HandleFootstep( EFoot::Left);
			FootstepsComponent->HandleFootstep( EFoot::Right);
		}
	}
}
