// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "DamageInfo.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None,
	Physical,
	Magical
};



UENUM(BlueprintType)
enum class EDamageElement : uint8
{
	// After adding new element: update GetAllElements
	None,
	Fire,
	Ice,
	Lightning
};

USTRUCT(BlueprintType, Blueprintable)
struct FDamageInfo
{
	GENERATED_BODY()

	// Make sure all elements are added
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EDamageElement, float> PhysicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EDamageElement, float> MagicDamage;

	static TArray<EDamageElement> GetAllElements();
public:

	FDamageInfo();

	void Scale(float);

	float GetTotalDamage() const;
};