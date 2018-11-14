// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment.h"


// Sets default values for this component's properties
UEquipment::UEquipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	UE_LOG(LogTemp, Warning, TEXT("Size: %d"), CurrentEquipment.Num());
}


// Called every frame
void UEquipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipment::AttackStats(UCharacterStats * S)
{
	ManipulatedStats = S;
}

bool UEquipment::Equip(const FItemInfo & Item)
{
	if(!CurrentEquipment.Contains(Item.ItemType))
		return false;

	if(CurrentEquipment[Item.ItemType].ItemType != EItemType::None)
		return false;

	CurrentEquipment[Item.ItemType] = Item;

	for(auto I : CurrentEquipment)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot: %d, Item: %s"), (int32)I.Key,  *I.Value.ItemName.ToString());
	}

	// TODO: manipulate stats
	OnChange.Broadcast();

	return true;
}

bool UEquipment::Unequip(EItemType Slot)
{
	if(!CurrentEquipment.Contains(Slot) || CurrentEquipment[Slot].ItemType == EItemType::None)
		return false;

	CurrentEquipment[Slot] = FItemInfo::GetEmptyItem();
	OnChange.Broadcast();

	// TODO: manipulate stats

	return true;
}

FItemInfo UEquipment::GetItem(const EItemType Slot) const
{
	if(!CurrentEquipment.Contains(Slot))
		return FItemInfo::GetEmptyItem();
	return CurrentEquipment[Slot];
}
