// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Includes.h"
#include "Enemies/BaseEnemy.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Decorators/BTDecorator_IsAtLocation.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UBTD_IsInRange : public UBTDecorator
{
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	

public:
	UPROPERTY(EditAnywhere, Category = "Combat")
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "Combat")
		FBlackboardKeySelector RangeKey;
};
