// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment.h"
#include "Characters/Inventory.h"
#include "Components/CharacterStats.h"
#include "Items/PickupItem.h"


// Sets default values for this component's properties
UEquipment::UEquipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipment::BeginPlay()
{
	Super::BeginPlay();

	// ...

	TArray<EItemType> Types = FItemInfo::GetEqItemTypes();


	for(auto T : Types)
	{
		CurrentEquipment.Add(T, FItemInfo::GetEmptyItem());
	}
}


// Called every frame
void UEquipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipment::AttachStats(UCharacterStats * S)
{
	ManipulatedStats = S;
}

void UEquipment::AttachInventory(UInventory * Inv)
{
	Backpack = Inv;
}

FItemInfo UEquipment::Equip(const FItemInfo & Item)
{
	if(!CurrentEquipment.Contains(Item.ItemType))
		return FItemInfo::GetEmptyItem();

	FItemInfo Previous = Unequip(Item.ItemType);
	CurrentEquipment[Item.ItemType] = Item;
	for(auto I : CurrentEquipment)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot: %d, Item: %s"), (int32)I.Key,  *I.Value.ItemName.ToString());
	}

	ManipulateStats(CurrentEquipment[Item.ItemType], true);
	OnChange.Broadcast();

	return Previous;
}

FItemInfo UEquipment::Unequip(EItemType Slot)
{
	if(!CurrentEquipment.Contains(Slot) || CurrentEquipment[Slot].ItemType == EItemType::None)
		return FItemInfo::GetEmptyItem();

	if(CurrentEquipment[Slot].ItemType != EItemType::None)
		ManipulateStats(CurrentEquipment[Slot], false);
	FItemInfo Removed = GetItem(Slot);
	CurrentEquipment[Slot] = FItemInfo::GetEmptyItem();
	// SendToBackpack(Removed);
	OnChange.Broadcast();

	return Removed;
}

void UEquipment::SendToBackpack(const FItemInfo & Item)
{
	if(!Backpack)
		return;
	Backpack->AddForced(Item);
}

void UEquipment::ManipulateStats(const FItemInfo & Item, bool bEquip)
{
	if(!ManipulatedStats)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Manipulating, number: %i"), Item.GrantedMods.Mods.Num());

	if(bEquip)
		ManipulatedStats->AddList(Item.GrantedMods);
	else
		ManipulatedStats->RemoveList(Item.GrantedMods);
}

bool UEquipment::HasItem(const EItemType Slot) const
{
	if(!CurrentEquipment.Contains(Slot))
		return false;
	return CurrentEquipment[Slot].ItemType != EItemType::None;
}

FItemInfo UEquipment::GetItem(const EItemType Slot) const
{
	if(!CurrentEquipment.Contains(Slot))
		return FItemInfo::GetEmptyItem();
	return CurrentEquipment[Slot];
}
