// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkills/BaseSkill.h"
#include "Includes.h"
#include "Components/DetectionSphere.h"
#include "ActiveSkills/BaseProjectile.h"
#include "BasicRangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UBasicRangedAttack : public UBaseSkill
{
	GENERATED_BODY()

	virtual void BeginPlay();

	virtual bool Use_Implementation(class ABaseEntity * User, class ABaseEntity * Target, FVector TargetLocation) override;

	virtual void Cancel_Implementation() override;

	virtual void Finish_Implementation() override;



protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skills")
		TSubclassOf<ABaseProjectile> ProjectileClass;
	
private:

	UDetectionSphere * RangeSphere = nullptr;

	ABaseEntity * CurrentUser = nullptr;
	ABaseEntity * CurrentTarget = nullptr;
	FVector CurrentTargetLocation;
	
	
};
