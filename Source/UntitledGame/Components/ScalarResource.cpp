// Fill out your copyright notice in the Description page of Project Settings.

#include "ScalarResource.h"


// Sets default values for this component's properties
UScalarResource::UScalarResource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScalarResource::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if(GetWorld() && bShouldRegen)
		GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &UScalarResource::TickRegen, RegenDelay, true);
	
}


float UScalarResource::Get() const
{
	return CurrentValue;
}

float UScalarResource::GetMax() const
{
	return MaxValue;
}

void UScalarResource::ChangeMax(float NewValue, bool bKeepRatio)
{
	float Precentage = GetPercentage();
	MaxValue = FMath::Max<float>(0.f, NewValue);
	if(bKeepRatio)
		CurrentValue = MaxValue * Precentage;
}

float UScalarResource::GetPercentage() const
{
	if(FMath::IsNearlyEqual(MaxValue, 0.f))
		return 1.f;
	return CurrentValue / MaxValue;
}


float UScalarResource::Remove(float ToRemove)
{
	float Previous = Get();
	CurrentValue = FMath::Clamp<float>(CurrentValue - ToRemove, 0.f, MaxValue);
	return Previous - Get();
}

float UScalarResource::Restore(float ToRestore)
{
	float Previous = Get();
	CurrentValue = FMath::Clamp<float>(CurrentValue + ToRestore, 0, GetMax());
	return Get() - Previous;
}

void UScalarResource::SetRegenRate(float NewRate)
{
	RegenRate = NewRate;
}

float UScalarResource::GetRegenRate() const
{
	return RegenRate;
}

// Called every frame
void UScalarResource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UScalarResource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(!GetWorld())
		return;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


void UScalarResource::TickRegen()
{
	Restore(GetRegenRate() * RegenDelay);
}
