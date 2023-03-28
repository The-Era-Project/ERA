// Fill out your copyright notice in the Description page of Project Settings.


#include "ERA_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UERA_GameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
