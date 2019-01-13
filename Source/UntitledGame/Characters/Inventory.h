// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "Items/ItemInfo.h"
#include "Items/PickupItem.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryDelegate, class UInventory *, Inventory, bool, PickedUp, FItemInfo, ItemInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNTITLEDGAME_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	// Tries to add. If no place returns false, otherwise adds and returns true
	UFUNCTION(BlueprintCallable, Category = "Items")
		bool AddItem(FItemInfo ToAdd);

	// Tries to add item. If no place spawns it in world and returns false. Otherwise adds and returns true
	UFUNCTION(BlueprintCallable, Category = "Items")
		bool AddForced(FItemInfo ToAdd);

	// Returns previous item
	UFUNCTION(BlueprintCallable, Category = "Items")
		FItemInfo Replace(int32 Index, FItemInfo NewItem);

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool RemoveItemAtIndex(int32 Index);

	// only for stackables
	UFUNCTION(BlueprintCallable, Category = "Items")
		bool RemoveItemAtIndexStack(int32 Index, int32 Count);

	// Helper funtionc to spawn item that's not in inventory
	UFUNCTION(BlueprintCallable, Category = "Items")
		void DropForced(FItemInfo & ToDrop);

	// Removes item and drops to the ground
	UFUNCTION(BlueprintCallable, Category = "Items")
		bool DropItem(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void DropAll();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void SwapItems(int32 Index, int32 OtherIndex);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void PrintItems();

	UFUNCTION(BlueprintPure, Category = "Items")
		int32 GetFirstEmptySlot() const;

	// clamps index to inventory size
	UFUNCTION(BlueprintCallable, Category = "Items")
		FItemInfo GetItem(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool AddMoney(FItemInfo Money);

	UFUNCTION(BlueprintPure, Category = "Items")
		float GetMoneyCount() const;

	UFUNCTION(BlueprintPure, Category = "Items")
		float GetRareMoneyCount() const;

	UFUNCTION(BlueprintPure, Category = "Items")
		bool HasEmptyPlace(FItemInfo & Stackable) const;
	UFUNCTION(BlueprintPure, Category = "Items")
		bool HasItemAtIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Items")
		void EmptyInventory();

	// broadcasted every time inventory state is changed
	UPROPERTY(BlueprintReadOnly, BlueprintAssignable, Category = "Items")
		FInventoryDelegate OnPickup;
	UPROPERTY(BlueprintReadOnly, BlueprintAssignable, Category = "Items")
		FInventoryDelegate OnDrop;
public:

	UFUNCTION(BlueprintPure, Category = "Items")
		int32 GetNumberOfItems() const;

	// resets inventory and returns new capacity
		int32 SetCapacity(int32 NewSize);

	UFUNCTION(BlueprintPure, Category = "Items")
		int32 GetCapacity() const;

	UFUNCTION(BlueprintPure, Category = "Items")
		ACharacter * GetInventoryOwner() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		int32 Capacity = 16;

	TSubclassOf<APickupItem> DroppedItemClass;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
		TArray<FItemInfo> Content;
private:

	// Magic number, don't change in BP
	float DropRange = 100.f;

	void SetPickupItemClass();

	float CurrentMoney = 0;

	float CurrentRareMoney = 0;

public:
	int32 CurrentSize = 0;
};
