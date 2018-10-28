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
	
}


// Called every frame
void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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