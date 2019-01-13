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

	bool ReceiveModifier(const FStatsModifier & Mod);

	bool RemoveModifier(const FStatsModifier & Mod);

protected:

	mutable bool bChanged = false;

	TArray<const FStatsModifier *> AppliedMods;

	float Base = 0.f;

	UPROPERTY(VisibleAnywhere)
		mutable float Value = 0.f;
	
	
};
