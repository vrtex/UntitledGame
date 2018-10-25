// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Includes.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Inventory.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Setup")
		void UpdateInventory(class UInventory * ChangedInventory, bool bInserted, FItemInfo ChangedItem); // throwaway arguments, for compatibility woth delegate
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Setup")
		void SetupInventory(class UInventory * Inventory);

	void SetupInventory_Implementation(class UInventory * OwningCharacter);

	
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Items")
		UInventory * OwnerInv = nullptr;
};
