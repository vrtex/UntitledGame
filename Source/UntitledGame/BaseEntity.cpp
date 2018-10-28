// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEntity.h"


// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStats = CreateDefaultSubobject<UCharacterStats>(FName("Character Stats"));
	Health = CreateDefaultSubobject<UHealthComponent>(FName("Health component"));
}

// Called when the game starts or when spawned
void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEntity::SetTeamLabel(uint8 OtherLabel)
{
	CurrentTeam = OtherLabel;
}

uint8 ABaseEntity::GetTeamLabel() const
{
	return CurrentTeam;
}

ETeamRelation ABaseEntity::GetRelationTowards(uint8 OtherLabel) const
{
	if(GetTeamLabel() == GenericNeutralTeam || OtherLabel == GenericNeutralTeam)
		return ETeamRelation::Neutral;
	if(GetTeamLabel() == OtherLabel)
		return ETeamRelation::Friendly;
	return ETeamRelation::Hostile;
}

bool ABaseEntity::DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
{
	for(TPair<EDamageElement, float> PartialDamage : Damage.PhysicalDamage)
	{
		float ThisElement = PartialDamage.Value * (1.f - GetResistTo(PartialDamage.Key, EDamageType::Physical));
		DealtDamage.PhysicalDamage[PartialDamage.Key] = ThisElement;
	}

	for(TPair<EDamageElement, float> PartialDamage : Damage.MagicDamage)
	{
		float ThisElement = PartialDamage.Value * (1.f - GetResistTo(PartialDamage.Key, EDamageType::Magical));
		DealtDamage.MagicDamage[PartialDamage.Key] = ThisElement;
	}
	Health->Remove(DealtDamage);
	return !FMath::IsNearlyEqual(0.f, DealtDamage.GetTotalDamage());
}

float ABaseEntity::GetResistTo(EDamageElement Element, EDamageType Type) const
{
	// TODO: get resists depending on type
	return CharacterStats->GetResistTo(Element);
}

float ABaseEntity::GetHealth() const
{
	return Health ? Health->Get() : 0.f;
}

float ABaseEntity::GetMaxHealth() const
{
	return Health ? Health->GetMax() : 0.f;
}

float ABaseEntity::GetHealthPercentage() const
{
	return Health ? Health->GetPercentage() : 0.f;
}


bool ABaseEntity::IsDead() const
{
	return FMath::IsNearlyEqual(0.f, GetHealthPercentage());
}