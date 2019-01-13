// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStat.h"


void UCharacterStat::SetBase(float nBase)
{
	Base = nBase;
	bChanged = true;
}


float UCharacterStat::GetValue() const
{
	if(!bChanged)
		return Value;

	Value = Base;
	float FlatIncrease = 0.f;
	float IncreaseValue = 1.f;
	TArray<float> MoreMultipliers;

	for(const auto & M : AppliedMods)
	{
		switch(M->Type)
		{
		case EModType::Flat:
			FlatIncrease += M->Value;
			break;
		case EModType::Increased:
			IncreaseValue += M->Value;
			break;
		case EModType::More:
			MoreMultipliers.Add(M->Value + 1.f);
			break;
		default:
			break;
		}
	}

	Value += FlatIncrease;
	Value *= IncreaseValue;
	for(auto Multiplier : MoreMultipliers)
		Value *= Multiplier;

	bChanged = false;
	return Value;
}

void UCharacterStat::ReceiveModifier(const FStatsModifier & Mod)
{
	if(AppliedMods.Contains(&Mod))
		return;

	AppliedMods.Add(&Mod);
	bChanged = true;
}

void UCharacterStat::RemoveModifier(const FStatsModifier & Mod)
{
	if(!AppliedMods.Contains(&Mod))
		return;

	AppliedMods.Remove(&Mod);
	bChanged = true;
}