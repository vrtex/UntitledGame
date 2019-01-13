// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/StatsModifier.h"
#include "CharacterStat.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UCharacterStat : public UObject
{
	GENERATED_BODY()

public:

	void SetBase(float nBase);

	UFUNCTION(BlueprintPure)
		float GetValue() const;

	void ReceiveModifier(const FStatsModifier & Mod);

	void RemoveModifier(const FStatsModifier & Mod);

private:

	mutable bool bChanged = false;

	TArray<const FStatsModifier *> AppliedMods;

	float Base = 0.f;

	mutable float Value = 0.f;
	
	
};
