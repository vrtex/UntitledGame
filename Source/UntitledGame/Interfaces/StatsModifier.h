// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "StatsModifier.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EStatField : uint8
{
	None,
	MaxHealth,
	MaxMana
};

UENUM(BlueprintType)
enum class EModType : uint8
{
	None,
	Flat,
	Increased,
	More
};

USTRUCT(BlueprintType, Blueprintable)
struct FStatsModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EStatField Field = EStatField::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EModType Type = EModType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value = 0.f;
};



USTRUCT(BlueprintType, Blueprintable)
struct FStatsModifierList
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStatsModifier> Mods;

};
