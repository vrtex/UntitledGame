// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStats.h"


// Sets default values for this component's properties
UCharacterStats::UCharacterStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	auto Elements = FDamageInfo::GetAllElements();
	for(auto E : Elements)
		ElementalResists.Add(E, 0.f);

}

// Called when the game starts
void UCharacterStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StatList.Add(EStatField::MaxHealth, NewObject<UCharacterStat>(this, UCharacterStat::StaticClass()));
	StatList[EStatField::MaxHealth]->SetBase(80.f);
	StatList.Add(EStatField::MaxMana, NewObject<UCharacterStat>(this, UCharacterStat::StaticClass()));
	StatList[EStatField::MaxMana]->SetBase(100.f);
	
}


// Called every frame
void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStats::AddLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("SDOUGSDUIGNJHBIN"));
	MaxHealth += 10.f;
	MaxMana += 20.f;
	OnChange.Broadcast();
}
/*
void UCharacterStats::AddMod(const FStatsModifier & Mod)
{
	switch(Mod.Field)
	{
	case EStatField::MaxHealth:
		SetMaxHealth(GetMaxHealth() + Mod.Min);
		break;
	case EStatField::MaxMana:
		SetMaxMana(GetMaxMana() + Mod.Min);
		break;
	case EStatField::None:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown stat field UCharacterStats::AddMod"));
		break;
	}
}

void UCharacterStats::RemoveMod(const FStatsModifier & Mod)
{
	switch(Mod.Field)
	{
	case EStatField::MaxHealth:
		SetMaxHealth(GetMaxHealth() - Mod.Min);
		break;
	case EStatField::MaxMana:
		UE_LOG(LogTemp, Warning, TEXT("Removing mana"));
		SetMaxMana(GetMaxMana() - Mod.Min);
		break;
	case EStatField::None:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown stat field UCharacterStats::AddMod"));
		break;
	}
}
*/

void UCharacterStats::AddList(const FStatsModifierList & List)
{
	for(const auto & M : List.Mods)
	{
		if(!StatList.Contains(M.Field))
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to add mod to unexisting stat. UCharacterStats::AddList"));
			continue;
		}

		StatList[M.Field]->ReceiveModifier(M);
	}
}

void UCharacterStats::RemoveList(const FStatsModifierList & List)
{
	for(const auto & M : List.Mods)
	{
		if(!StatList.Contains(M.Field))
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to remove mod from unexisting stat. UCharacterStats::RemoveList"));
			continue;
		}

		StatList[M.Field]->RemoveModifier(M);
	}
}

void UCharacterStats::RecalculateStats()
{
}

void UCharacterStats::SetMaxHealth(float NewMax)
{
	MaxHealth = FMath::Max<float>(0.f, NewMax);
	StatList[EStatField::MaxMana]->SetBase(NewMax);
	OnChange.Broadcast();
}

float UCharacterStats::GetMaxHealth() const
{
	return StatList[EStatField::MaxHealth]->GetValue();
}

void UCharacterStats::SetMaxMana(float NewMax)
{
	MaxMana = FMath::Max<float>(0.f, NewMax);
	StatList[EStatField::MaxMana]->SetBase(NewMax);
	OnChange.Broadcast();
}

float UCharacterStats::GetMaxMana() const
{
	return StatList[EStatField::MaxMana]->GetValue();
}

float UCharacterStats::GetResistTo(EDamageElement Element) const
{
	if(!ElementalResists.Contains(Element))
	{
		const UEnum * EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDamageElement"), true);
		if(EnumPtr)
		{
			FString Name = EnumPtr->GetNameByIndex((int64)(Element)).ToString();
			UE_LOG(LogTemp, Warning, TEXT("Element %s isn't in character stats sheet. Fix this boo-boo"), *Name)
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trying to read unknown element from character sheet. Shiet"))
		}
		return 0.f;
	}
	return ElementalResists[Element];
}

void UCharacterStats::SetResistTo(EDamageElement Element, float NewValue)
{
	ElementalResists[Element] = NewValue;
	OnChange.Broadcast();
}