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
	float IncreaseValue = 100.f;
	TArray<float> MoreMultipliers;

	for(const auto & M : AppliedMods)
	{
		switch(M->Type)
		{
		case EModType::None:
			UE_LOG(LogTemp, Warning, TEXT("Boo-boo, type is none"));
			break;
		case EModType::Flat:
			FlatIncrease += M->Value;
			break;
		case EModType::Increased:
			IncreaseValue += M->Value;
			break;
		case EModType::More:
			MoreMultipliers.Add(M->Value + 100.f);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown mod type"));
			break;
		}
	}


	Value += FlatIncrease;
	Value *= IncreaseValue / 100.f;
	for(auto Multiplier : MoreMultipliers)
		Value *= Multiplier / 100.f;

	bChanged = false;
	return Value;
}

bool UCharacterStat::ReceiveModifier(const FStatsModifier & Mod)
{
	if(AppliedMods.Contains(&Mod))
		return false;

	AppliedMods.Add(&Mod);
	bChanged = true;
	return true;
}

bool UCharacterStat::RemoveModifier(const FStatsModifier & Mod)
{
	if(!AppliedMods.Contains(&Mod))
		return false;
	AppliedMods.Remove(&Mod);
	bChanged = true;
	return true;
}