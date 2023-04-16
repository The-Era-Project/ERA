// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryList.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Actors/ItemActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemLog.h"
#include "GameplayTagsManager.h"

FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::UnequipItemActorTag;
FGameplayTag UInventoryComponent::DropItemActorTag;
FGameplayTag UInventoryComponent::EquipNextItemActorTag;

static TAutoConsoleVariable<int32> CVarShowInventory(
	TEXT("ShowDebugInventory"),
	0,
	TEXT("Draws debug info about inventory.")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	
	static bool bHandledAddingTags = false;
	if (!bHandledAddingTags)
	{
		bHandledAddingTags = true;
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryTags);
	}
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);

	}
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItemInstance(InItemInstance);
	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (const auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance->ItemStaticDataClass == InItemStaticDataClass)
			{
				Item.ItemInstance->OnEquipped(GetOwner());

				CurrentItem = Item.ItemInstance;

				break;
			}
		}
	}
}

void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		for (const auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				Item.ItemInstance->OnEquipped(GetOwner());
				CurrentItem = Item.ItemInstance;
				break;
			}
		}
	}
}


void UInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnUnequipped();
			CurrentItem = nullptr;
		}
	}
}

void UInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnDropped();
			RemoveItem(CurrentItem->ItemStaticDataClass);
			CurrentItem = nullptr;
		}
	}
}

void UInventoryComponent::EquipNext()
{

	TArray<FInventoryListItem>& Items = InventoryList.GetItemsRef();

	const bool bNoItems = Items.Num() == 0;
	const bool bOneAndEquipped = Items.Num() == 1 && CurrentItem;

	if(bNoItems || bOneAndEquipped) return;
	
	// If there's only one item and it's not equipped, equip it.
	if (Items.Num() == 1 && !CurrentItem) {
		EquipItemInstance(Items[0].ItemInstance);
		return;
	}

	UInventoryItemInstance* TargetItem = CurrentItem;

	for (const auto Item : Items)
	{
		if (Item.ItemInstance->GetItemStaticData()->bCanBeEquipped)
		{
			if (Item.ItemInstance != CurrentItem)
			{
				TargetItem = Item.ItemInstance;
				break;
			}
		}
	}

	if (CurrentItem)
	{
		
		if (TargetItem == CurrentItem)
		{
			return;
		}

		UnequipItem();
	}
	
	EquipItemInstance(TargetItem);
}

UInventoryItemInstance* UInventoryComponent::GetEquippedItem() const
{
	return CurrentItem;
}

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEvent(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

void UInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	// Initialise tags
	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem"), TEXT("Equip item form item actor event"));
	UInventoryComponent::UnequipItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.UnequipItem"), TEXT("Unequip item form item actor event"));
	UInventoryComponent::DropItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItem"), TEXT("Drop item form item actor event"));
	UInventoryComponent::EquipNextItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNextItem"), TEXT("Equip next item form item actor event"));

	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void UInventoryComponent::HandleGameplayEvent(const FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == EquipItemActorTag)
		{
			if (const UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInstance(const_cast<UInventoryItemInstance*>(ItemInstance));

				if (Payload.Instigator)
				{
					const_cast<AActor*>(Payload.Instigator.Get())->Destroy();
				}
			}
		}
		else if (EventTag == EquipNextItemActorTag)
		{
			EquipNext();
		}
		else if (EventTag == DropItemActorTag)
		{
			DropItem();
		}
		else if (EventTag == UnequipItemActorTag)
		{
			UnequipItem();
		}
	}
}

void UInventoryComponent::ServerHandleGameplayEvent_Implementation(const FGameplayEventData Payload)
{
	HandleGameplayEvent(Payload);
}

bool UInventoryComponent::ServerHandleGameplayEvent_Validate(const FGameplayEventData Payload)
{
	// You can add any validation logic here
	return true;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	const bool bShowDebug = CVarShowInventory.GetValueOnGameThread() != 0;

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;
		const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData();

		if (IsValid(ItemInstance) && IsValid(ItemStaticData))
		{
			if (bShowDebug && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString()));
			}
		}
	}	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, CurrentItem);
}

