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
		EStatField Field;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EModType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Max;
};



USTRUCT(BlueprintType, Blueprintable)
struct FStatsModifierList
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStatsModifier> Mods;

};
