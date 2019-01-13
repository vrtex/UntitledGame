// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSkill.h"
#include "BaseEntity.h"


// Sets default values for this component's properties
UBaseSkill::UBaseSkill()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBaseSkill::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UBaseSkill::Use_Implementation(ABaseEntity * User, ABaseEntity * Target, FVector TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor %s tries to use base skill. This skill deas nothing. It's a bad idea"), *User->GetName());
	return false;
}

void UBaseSkill::Cancel_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill %s has no cancel implementation or it's a base skill."), *GetName());
}

void UBaseSkill::Finish_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill %s has no finish implementation or it's a base skill."), *GetName());
}

UTexture2D * UBaseSkill::GetSkillIcon() const
{
	return SkillIcon;
}

float UBaseSkill::GetRange() const
{
	return Range;
}

float UBaseSkill::NeedsTarget() const
{
	return bNeedsTarget;
}