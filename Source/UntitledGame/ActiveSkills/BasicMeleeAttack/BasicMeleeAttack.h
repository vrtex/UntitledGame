// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkills/BaseSkill.h"
#include "Includes.h"
#include "Components/DetectionSphere.h"
#include "BasicMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UBasicMeleeAttack : public UBaseSkill
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;


	virtual bool Use_Implementation(class ABaseEntity * User, class ABaseEntity * Target, FVector TargetLocation);

private:

	UFUNCTION()
		void EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UDetectionSphere * RangeSphere = nullptr;
	
};
