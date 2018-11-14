// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"




void UInventoryWidget::SetupInventory_Implementation(ABaseCharacter * Character)
{
	if(!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Error during inventory setup. No character"));
		return;
	}
	UInventory * Inventory = Character->Backpack;
	UEquipment * Eq = Character->Equipment;
	if(!Inventory || !Eq)
	{
		UE_LOG(LogTemp, Error, TEXT("Error during inventory setup. No inventory or eq"));
		return;
	}
	Owner = Character;

	OwnerInv = Inventory;
	Inventory->OnPickup.AddDynamic(this, &UInventoryWidget::UpdateInventory);
	Inventory->OnDrop.AddDynamic(this, &UInventoryWidget::UpdateInventory);

	OwnerEq = Eq;
}
