// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageInfo.h"


FDamageInfo::FDamageInfo()
{
	auto Elements = FDamageInfo::GetAllElements();
	for(auto E : Elements)
	{
		PhysicalDamage.Add(E, 0.f);
		MagicDamage.Add(E, 0.f);
	}
}

void FDamageInfo::Scale(float Multiplier)
{
	for(auto Element : FDamageInfo::GetAllElements())
	{
		PhysicalDamage[Element] = PhysicalDamage[Element] * Multiplier;
		MagicDamage[Element] = MagicDamage[Element] * Multiplier;
	}
}

float FDamageInfo::GetTotalDamage() const
{
	float Sum = 0.0f;
	for(TPair<EDamageElement, float> PartialDamage : PhysicalDamage)
		Sum += PartialDamage.Value;
	for(TPair<EDamageElement, float> PartialDamage : MagicDamage)
		Sum += PartialDamage.Value;

	return Sum;
}

TArray<EDamageElement> FDamageInfo::GetAllElements()
{
	TArray<EDamageElement> ToReturn;
	ToReturn.Add(EDamageElement::None);
	ToReturn.Add(EDamageElement::Fire);
	ToReturn.Add(EDamageElement::Lightning);
	ToReturn.Add(EDamageElement::Ice);
	return ToReturn;
}