// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActiveSkills/BaseSkill.h"
#include "Includes.h"
#include "SkillSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillSetFelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API USkillSet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillSet();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	FSkillSetFelegate OnChange;

	bool ChangeSkill(ESkillSlot Slot, TSubclassOf<UBaseSkill> NewSkill);

	bool UseSkill(ESkillSlot Slot, class ABaseEntity * User, class ABaseEntity * Target, const FVector & TargetLocation);

	UFUNCTION(BlueprintPure, Category = "Skills")
		UBaseSkill * GetSkill(ESkillSlot Slot) const;

	UFUNCTION(BlueprintPure, Category = "Skills")
		const TMap<ESkillSlot, UBaseSkill *> & GetCurrentSkills();

private:
	void AddSkills();

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		TMap<ESkillSlot, TSubclassOf<UBaseSkill>> SkillClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
		TMap<ESkillSlot, UBaseSkill *> CurrentSkills;
};
