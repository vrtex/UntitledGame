// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::Get() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMax() const
{
	return MaxHealth;
}

float UHealthComponent::GetPercentage() const
{
	if(FMath::IsNearlyEqual(MaxHealth, 0.f))
		return 0.f;
	return CurrentHealth / MaxHealth;
}


bool UHealthComponent::Remove(FDamageInfo & Damage)
{
	if(CurrentHealth < Damage.GetTotalDamage())
	{
		float Multiplier = CurrentHealth / Damage.GetTotalDamage();
		Damage.Scale(Multiplier);
	}

	CurrentHealth = FMath::Clamp<float>(CurrentHealth - Damage.GetTotalDamage(), 0.f, MaxHealth);
	return !FMath::IsNearlyEqual(Damage.GetTotalDamage(), 0.f);
}