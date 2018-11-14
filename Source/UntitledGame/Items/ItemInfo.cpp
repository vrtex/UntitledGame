// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemInfo.h"

TArray<EItemType> FItemInfo::GetAllItemTypes()
{
	TArray<EItemType> Types = {
		EItemType::None,
		EItemType::Money,
		EItemType::RareMoney,
		EItemType::WeaponModifier,
		EItemType::ShieldGenerator,
		EItemType::Gloves,
		EItemType::Boots,
		EItemType::Implant,
		EItemType::Relic,
		EItemType::Consumable,
		EItemType::Other
	};
	return Types;
}

TArray<EItemType> FItemInfo::GetEqItemTypes()
{
	TArray<EItemType> Types = GetAllItemTypes();
	Types.Remove(EItemType::Other);
	Types.Remove(EItemType::Consumable);
	Types.Remove(EItemType::None);
	Types.Remove(EItemType::RareMoney);
	Types.Remove(EItemType::Money);
	return Types;
}

FItemInfo FItemInfo::GetEmptyItem()
{
	FItemInfo EmptyItem;
	EmptyItem.ItemType = EItemType::None;
	return EmptyItem;
}