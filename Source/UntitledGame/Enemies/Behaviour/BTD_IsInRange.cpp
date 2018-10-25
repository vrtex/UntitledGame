// Fill out your copyright notice in the Description page of Project Settings.

#include "BTD_IsInRange.h"

bool UBTD_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	APawn * Pawn = OwnerComp.GetAIOwner()->GetPawn();
	ABaseEnemy * ControlledEnemy = Cast<ABaseEnemy>(Pawn);
	APawn * Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	return ControlledEnemy ? ControlledEnemy->AttackRangeSphere->IsOverlappingActor(Target) : false;
	//return (Pawn && Target) ? FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation()) < Range : false;
}