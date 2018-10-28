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
		return false;

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
	return true;
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