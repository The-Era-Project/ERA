// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_InventoryAbility.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "ERAGameTypes.h"
#include "Actors/ItemActor.h"
#include "Inventory/WeaponItemActor.h"

void UGA_InventoryAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
}

UInventoryComponent* UGA_InventoryAbility::GetInventoryComponent() const
{
	return InventoryComponent;
}

UInventoryItemInstance* UGA_InventoryAbility::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

const UItemStaticData* UGA_InventoryAbility::GetEquippedItemStaticData() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemStaticData() : nullptr;
}

const UWeaponItemStaticData* UGA_InventoryAbility::GetEquippedWeaponItemStaticData() const
{
	return Cast<UWeaponItemStaticData>(GetEquippedItemStaticData());
}

AItemActor* UGA_InventoryAbility::GetEquippedItemActor() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemActor() : nullptr;
}

AWeaponItemActor* UGA_InventoryAbility::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}
