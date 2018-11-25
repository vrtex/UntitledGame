// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "ActiveSkills/BaseSkill.h"
#include "ItemInfo.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	// After changing: update static getter in FItemInfo
	None,
	Money,
	RareMoney,
	WeaponModifier,
	ShieldGenerator,
	Gloves,
	Boots,
	Implant,
	Relic,
	Consumable,
	Other
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemInfo
{
	GENERATED_BODY()

	static TArray<EItemType> GetAllItemTypes();
	
	static TArray<EItemType> GetEqItemTypes();

	static FItemInfo GetEmptyItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType ItemType = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStackable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 StackSize = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxStackSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemName = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * ItemIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UBaseSkill> GrantedSkill;

	// list of modifiers; might be empty

};