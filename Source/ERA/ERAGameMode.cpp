// Copyright Epic Games, Inc. All Rights Reserved.

#include "ERAGameMode.h"
#include "ERACharacter.h"
#include "UObject/ConstructorHelpers.h"

AERAGameMode::AERAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
