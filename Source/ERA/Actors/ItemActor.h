// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInstance.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"


UCLASS()
class ERA_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void Init(UInventoryItemInstance* InInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	UInventoryItemInstance* ItemInstance = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
