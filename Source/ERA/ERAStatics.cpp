// Fill out your copyright notice in the Description page of Project Settings.


#include "ERAStatics.h"

#include "ShaderPrintParameters.h"

const UItemStaticData* UERAStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass)
{
	if(IsValid(ItemDataClass))
	{
		return GetDefault<UItemStaticData>(ItemDataClass);
	}
	return nullptr;
}
