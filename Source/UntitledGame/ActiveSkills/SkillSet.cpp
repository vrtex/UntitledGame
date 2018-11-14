// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSet.h"


// Sets default values for this component's properties
USkillSet::USkillSet()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USkillSet::BeginPlay()
{
	Super::BeginPlay();
	// ...

	AddSkills();
}

bool USkillSet::ChangeSkill(ESkillSlot Slot, TSubclassOf<UBaseSkill> NewSkill)
{
	if(!NewSkill.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set skill to none on actor %s"), *GetOwner()->GetName());
		return false;
	}

	UBaseSkill * CreatedSkill = NewObject<UBaseSkill>(this, NewSkill.Get());

	if(!CreatedSkill)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't create skill"));
		return false;
	}

	CreatedSkill->RegisterComponent();

	if(CurrentSkills.Contains(Slot) && IsValid(CurrentSkills[Slot]))
		CurrentSkills[Slot]->DestroyComponent();
	if(!CurrentSkills.Contains(Slot))
		CurrentSkills.Add(Slot, nullptr);

	CurrentSkills[Slot] = CreatedSkill;
	OnChange.Broadcast();
	return true;
}

bool USkillSet::UseSkill(ESkillSlot Slot, ABaseEntity * User, ABaseEntity * Target, const FVector & TargetLocation)
{
	return GetSkill(Slot) ? GetSkill(Slot)->Use(User, Target, TargetLocation) : false;
}

UBaseSkill * USkillSet::GetSkill(ESkillSlot Slot) const
{
	return CurrentSkills.Contains(Slot) ? CurrentSkills[Slot] : nullptr;
}

const TMap<ESkillSlot, UBaseSkill*>& USkillSet::GetCurrentSkills()
{
	return CurrentSkills;
}

void USkillSet::AddSkills()
{
	int8 SkillNumber = 0;
	for(auto SkillTemplate : SkillClasses)
	{
		bool bAdded = ChangeSkill(SkillTemplate.Key, SkillTemplate.Value);
		if(!bAdded)
		{
			UE_LOG(LogTemp, Error, TEXT("Skill number %d on actor %s was added to table, but has no class"), SkillNumber, *GetOwner()->GetName());
		}
		++SkillNumber;
	}
	/*
	if(PrimarySkillClass.Get())
	{
		PrimarySkill = NewObject<UBaseSkill>(this, PrimarySkillClass.Get());
		PrimarySkill->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No primary skill class"));
	}
	if(!PrimarySkill)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't create skill"));
	}
	*/
}