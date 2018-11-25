// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActiveSkills/BaseSkill.h"
#include "Includes.h"
#include "Items/ItemInfo.h"
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

	static ESkillSlot MapToSkill(EItemType Item);

	UPROPERTY(BlueprintAssignable, Category = "Items")
		FSkillSetFelegate OnChange;

	void SetDefaultAttack(TSubclassOf<UBaseSkill> Default);

	bool ChangeSkill(ESkillSlot Slot, TSubclassOf<UBaseSkill> NewSkill, bool bSendDelegate = true);

	void ClearSkill(ESkillSlot Slot, bool bSendDelegate = true);

	bool UseSkill(ESkillSlot Slot, class ABaseEntity * User, class ABaseEntity * Target, const FVector & TargetLocation);

	UFUNCTION(BlueprintPure, Category = "Skills")
		bool HasSkill(ESkillSlot Slot) const;

	UFUNCTION(BlueprintPure, Category = "Skills")
		UBaseSkill * GetSkill(ESkillSlot Slot) const;

	UFUNCTION(BlueprintPure, Category = "Skills")
		const TMap<ESkillSlot, UBaseSkill *> & GetCurrentSkills();

	void AttachEquipment(class UEquipment * Eq);

private:
	void AddSkills();

	UFUNCTION()
		void UpdateFromEqupiment();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		TMap<ESkillSlot, TSubclassOf<UBaseSkill>> SkillClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
		TMap<ESkillSlot, UBaseSkill *> CurrentSkills;

private:

	TSubclassOf<UBaseSkill> DefaultBasicAttack;

	class UEquipment * ObservedEq = nullptr;
};
