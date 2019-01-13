// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterLevel.h"
#include "Characters/BaseCharacter.h"


// Sets default values for this component's properties
UCharacterLevel::UCharacterLevel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterLevel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	LevelUpParticles = Cast<ABaseCharacter>(GetOwner())->GetLevelUpParticles();
}


// Called every frame
void UCharacterLevel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterLevel::AddXP(float ToAdd)
{
	CurrentXP += ToAdd;
	if(CurrentXP < MaxXP)
		return;

	float Leftover = CurrentXP - MaxXP;
	LevelUp();
	AddXP(Leftover);
}

void UCharacterLevel::LevelUp()
{
	if(LevelUpParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), 
			LevelUpParticles, 
			GetOwner()->GetActorTransform());
		UE_LOG(LogTemp, Warning, TEXT("KROWA"));
	}
	CurrentXP = 0.f;
	++CurrentLevel;
	OnLevelUp.Broadcast();
}

float UCharacterLevel::GetCurrentXP() const
{
	return CurrentXP;
}

float UCharacterLevel::GetMaxXP() const
{
	return MaxXP;
}

float UCharacterLevel::GetCurrentPercentage() const
{
	return GetCurrentXP() / GetMaxXP();
}

int32 UCharacterLevel::GetCurrentLevel() const
{
	return CurrentLevel;
}