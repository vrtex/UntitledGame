// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Includes.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/BaseEnemyController.h"
#include "BTT_AttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UBTT_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
		FBlackboardKeySelector Target;
	
};
