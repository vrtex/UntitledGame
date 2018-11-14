// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_AttackTarget.h"




EBTNodeResult::Type UBTT_AttackTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent * BBComp = OwnerComp.GetBlackboardComponent();
	if(!BBComp)
		return EBTNodeResult::Failed;

	APawn * Pawn = OwnerComp.GetAIOwner()->GetPawn();
	ABaseEnemy * ControlledEnemy = Cast<ABaseEnemy>(Pawn);
	AAIController * Controller = Cast<AAIController>(ControlledEnemy->GetController());
	if(!ControlledEnemy)
		return EBTNodeResult::Failed;

	ABaseEntity * TargetEntity = Cast<ABaseEntity>(BBComp->GetValueAsObject(Target.SelectedKeyName));
	if(!TargetEntity)
		return EBTNodeResult::Failed;
	/*
	if(FVector::Dist(TargetActor->GetActorLocation(), ControlledEnemy->GetActorLocation()) > ControlledEnemy->AttackRange)
		return EBTNodeResult::Failed;
	*/
	if(Controller)
		Controller->SetFocus(TargetEntity);
	ControlledEnemy->UseSkill(ESkillSlot::Primary, TargetEntity, FVector());
	// ControlledEnemy->Attack(TargetActor);
	return EBTNodeResult::Succeeded;
}