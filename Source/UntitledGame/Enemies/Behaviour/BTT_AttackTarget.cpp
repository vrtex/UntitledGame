// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_AttackTarget.h"




EBTNodeResult::Type UBTT_AttackTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent * BBComp = OwnerComp.GetBlackboardComponent();
	if(!BBComp)
		return EBTNodeResult::Failed;

	APawn * Pawn = OwnerComp.GetAIOwner()->GetPawn();
	ABaseEnemy * ControlledEnemy = Cast<ABaseEnemy>(Pawn);
	if(!ControlledEnemy)
		return EBTNodeResult::Failed;

	AActor * TargetActor = Cast<AActor>(BBComp->GetValueAsObject(Target.SelectedKeyName));
	if(!TargetActor)
		return EBTNodeResult::Failed;
	/*
	if(FVector::Dist(TargetActor->GetActorLocation(), ControlledEnemy->GetActorLocation()) > ControlledEnemy->AttackRange)
		return EBTNodeResult::Failed;
	*/

	ControlledEnemy->Attack(TargetActor);
	return EBTNodeResult::Succeeded;
}