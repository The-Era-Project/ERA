// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryList.h"
#include "Inventory/InventoryItemInstance.h"
#include "ERAGameTypes.h"

void FInventoryList::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(InItemStaticDataClass);
	MarkItemDirty(Item);

}

// Function to add an item instance to the inventory list
void FInventoryList::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	// Add a default-initialized FInventoryListItem to the Items array and get a reference to it
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();

	// Assign the input item instance (InItemInstance) to the ItemInstance member of the newly added FInventoryListItem
	Item.ItemInstance = InItemInstance;

	// Call the MarkItemDirty function with the newly added item as the argument
	// This might update some internal state or mark the item as requiring further processing
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData()->IsA(InItemStaticDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}
