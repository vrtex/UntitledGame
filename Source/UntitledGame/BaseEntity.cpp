// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEntity.h"


// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CharacterStats = CreateDefaultSubobject<UCharacterStats>(FName("Character Stats"));
	Health = CreateDefaultSubobject<UHealthComponent>(FName("Health component"));
	Mana = CreateDefaultSubobject<UManaComponent>(FName("Mana component"));
	SkillSet = CreateDefaultSubobject<USkillSet>(FName("Skill set"));
}

// Called when the game starts or when spawned
void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	if(BaseAttackSkillClass.Get() && SkillSet)
	{
		SkillSet->SetDefaultAttack(BaseAttackSkillClass);
		SkillSet->ChangeSkill(ESkillSlot::Primary, BaseAttackSkillClass);
	}
	else
	{
		if(!SkillSet)
		{
			UE_LOG(LogTemp, Error, TEXT("No skill set"));
		}

		UE_LOG(LogTemp, Error, TEXT("%s has no basic attack. No hands"), *GetName());
	}
	CharacterStats->OnChange.AddDynamic(this, &ABaseEntity::OnStatsChange);
	OnStatsChange();
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

TArray<AActor*> ABaseEntity::GetEnemiesInRange() const
{
	TArray<AActor*> Enemies;
	if(GetAttackRange() < 0.1f)
		return Enemies;
	TArray<AActor*> Temp;

	UGameplayStatics::GetAllActorsOfClass(this, ABaseEntity::StaticClass(), Temp);

	for(auto E : Temp)
	{
		ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(E);
		if(!AsBaseEntity || GetRelationTowards(AsBaseEntity->GetTeamLabel()) != ETeamRelation::Hostile)
			continue;
		if(FVector::Dist(E->GetActorLocation(), GetActorLocation()) > GetAttackRange())
			continue;
		Enemies.Add(E);
	}
	return Enemies;
}

float ABaseEntity::GetAttackRange() const
{
	return CurrentSkill ? CurrentSkill->GetRange() : 0.f;
}

void ABaseEntity::SetAttackRange(float NewRange)
{
	return;
}

void ABaseEntity::StopMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
	// SetCurrentState(EEntityState::Moving);
}

void ABaseEntity::MoveToLocation(FVector Target)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Target);
	SetCurrentState(EEntityState::Moving);
}

void ABaseEntity::MoveToActor(AActor * Target)
{
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Target);
	SetCurrentState(EEntityState::Moving);
}

void ABaseEntity::FaceLocation(const FVector Target)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		Target
	);
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;
	SetActorRotation(TargetRotation);
}

void ABaseEntity::FaceActor(const AActor * Target)
{
	if(!Target)
		return;
	FaceLocation(Target->GetActorLocation());
	/*
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		Target->GetActorLocation()
	);
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;
	SetActorRotation(TargetRotation);
	*/
}

bool ABaseEntity::ReceiveDamageBP_Implementation(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
{
	return ReceiveDamage(Damage, DealtDamage, DamageDealer, Instigator);
}

bool ABaseEntity::ReceiveDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
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
	if(GetHealth() < DealtDamage.GetTotalDamage())
		DealtDamage.Scale(DealtDamage.GetTotalDamage() / GetHealth());
	Health->Remove(DealtDamage.GetTotalDamage());
	return !FMath::IsNearlyEqual(0.f, DealtDamage.GetTotalDamage());
}

bool ABaseEntity::DealDamageToBP_Implementation(ABaseEntity * Target, const FDamageInfo & Damage, FDamageInfo & DealtDamage)
{
	return DealDamageTo(Target, Damage, DealtDamage);
}

bool ABaseEntity::DealDamageTo(ABaseEntity * Target, const FDamageInfo & Damage, FDamageInfo & DealtDamage)
{
	if(!Target)
		return false;
	return Target->ReceiveDamage(Damage, DealtDamage, this, GetController());
}

float ABaseEntity::RestoreHealthBP_Implementation(const float ToRestore, ABaseEntity * Healer)
{
	return RestoreHealth(ToRestore, Healer);
}

float ABaseEntity::RestoreHealth(const float ToRestore, ABaseEntity * Healer)
{
	return Health ? Health->Restore(ToRestore) : 0.f;
}

float ABaseEntity::RemoveManaBP_Implementation(float ToRemove)
{
	return RemoveMana(ToRemove);
}

float ABaseEntity::RemoveMana(float ToRemove)
{
	return Mana ? Mana->Remove(ToRemove) : 0.f;
}

float ABaseEntity::RestoreManaBP_Implementation(float ToRestore)
{
	return RestoreMana(ToRestore);
}

float ABaseEntity::RestoreMana(float ToRestore)
{
	return Mana ? Mana->Restore(ToRestore) : 0.f;
}

void ABaseEntity::OnStatsChange()
{
	if(!CharacterStats)
		return;
	if(Health)
	{
		Health->ChangeMax(CharacterStats->GetMaxHealth());
	}
	if(Mana)
	{
		Mana->ChangeMax(CharacterStats->GetMaxMana());
	}
}

void ABaseEntity::AddStatsModifiers(const FStatsModifierList & ToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding, count: %i"), ToAdd.Mods.Num());
}

void ABaseEntity::RemoveStatsModifiers(const FStatsModifierList & ToRemove)
{
	UE_LOG(LogTemp, Warning, TEXT("Removeing, count: %i"), ToRemove.Mods.Num());
}

void ABaseEntity::SetCurrentState(EEntityState NewState)
{
	CurrentState = NewState;
}

EEntityState ABaseEntity::GetCurrentState() const
{
	return CurrentState;
}

bool ABaseEntity::IsInCombat() const
{
	if(CurrentState == EEntityState::Idle
		|| CurrentState == EEntityState::Moving)
	return false;

	return true;
}

bool ABaseEntity::CanReact() const
{
	if(CurrentState == EEntityState::Idle
		|| CurrentState == EEntityState::Moving)
		return true;
	return false;
}

bool ABaseEntity::UseSkill(ESkillSlot ToUse, ABaseEntity * Target, FVector Location)
{
	UBaseSkill * NewSkill = GetSkill(ToUse);
	if(!NewSkill)
		return false;
	if(CurrentSkill != NewSkill && CurrentSkill)
		CurrentSkill->Cancel();
	
	CurrentSkill = NewSkill;
	SetAttackRange(CurrentSkill->GetRange());
	TargetActor = Target;
	TargetLocation = Location;
	return CurrentSkill->Use(this, Target, TargetLocation);
}

void ABaseEntity::FinishCurrentSkill()
{
	if(!CurrentSkill)
		return;

	CurrentSkill->Finish();
	CurrentSkill = nullptr;
}

USkillSet * ABaseEntity::GetSkillSet() const
{
	return SkillSet;
}

UBaseSkill * ABaseEntity::GetSkill(ESkillSlot Slot) const
{
	if(!SkillSet)
		return nullptr;
	return SkillSet->GetSkill(Slot);
}

void ABaseEntity::CancelCurrentSkill()
{
	if(!CurrentSkill)
		return;
	CurrentSkill->Cancel();
}

UCharacterStats * ABaseEntity::GetStats() const
{
	return CharacterStats;
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

float ABaseEntity::GetMana() const
{
	return Mana ? Mana->Get() : 0.f;
}

float ABaseEntity::GetMaxMana() const
{
	return Mana ? Mana->GetMax() : 0.f;
}

float ABaseEntity::GetManaPercentage() const
{
	return Mana ? Mana->GetPercentage() : 0.f;
}

bool ABaseEntity::IsDead() const
{
	return FMath::IsNearlyEqual(0.f, GetHealthPercentage());
}