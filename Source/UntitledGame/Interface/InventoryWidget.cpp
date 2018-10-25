// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"




void UInventoryWidget::SetupInventory_Implementation(UInventory * Inventory)
{
	if(!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Error during inventory setup. No inventory"));
		return;
	}
	OwnerInv = Inventory;
	Inventory->OnPickup.AddDynamic(this, &UInventoryWidget::UpdateInventory);
	Inventory->OnDrop.AddDynamic(this, &UInventoryWidget::UpdateInventory);
}
