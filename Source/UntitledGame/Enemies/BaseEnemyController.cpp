// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyController.h"
#include "UntitledGameGameModeBase.h"

void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseEnemyController::SetupBehaviour()
{

	AUntitledGameGameModeBase * GameMode = Cast<AUntitledGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		// UsedBlackboard = GameMode->BaseEnemyBlackboard;
		UBlackboardComponent * BBComp = GetBlackboardComponent();
		UseBlackboard(GameMode->BaseEnemyBlackboard, BBComp);

		RunBehaviorTree(GameMode->BaseEnemyBehaviourTree);
	}
	UBlackboardComponent * BBComp = GetBlackboardComponent();
	if(UsedBlackboardOverride)
		UseBlackboard(UsedBlackboardOverride, BBComp);
	else
	{
		if(GameMode)
			UseBlackboard(GameMode->BaseEnemyBlackboard, BBComp);
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find either base or overide Blackbard data for %s on %s"), *GetName(), *GetPawn()->GetName());
		}
	}


	if(BehaviourTreeOverride)
		RunBehaviorTree(BehaviourTreeOverride);
	else
	{
		if(GameMode)
			RunBehaviorTree(GameMode->BaseEnemyBehaviourTree);
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find either base or overide BT for %s on %s"), *GetName(), *GetPawn()->GetName());
		}
	}
	GetBlackboardComponent()->SetValueAsFloat(BaseEnemyAttackRangeKey, GetControledEnemy()->AttackRange);

}

void ABaseEnemyController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	Controlled = Cast<ABaseEnemy>(aPawn);
	if(!ensure(Controlled))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController %s has no pawn to controll"), *GetName());
		FGenericPlatformMisc::RequestExit(false);
		return;
	}

	// Controlled->AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyController::EnemyInRange);
	// Controlled->AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemyController::EnemyOutOfRange);
	Controlled->NoticeRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyController::EnemyDetected);
	Controlled->ForgetRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemyController::ForgetEnemy);

	SetupBehaviour();

}

ABaseEnemy * ABaseEnemyController::GetControledEnemy() const
{
	return Controlled;
}

void ABaseEnemyController::EnemyInRange(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor == Cast<AActor>(TargetedActor) && GetBlackboardComponent())
	{
		SetFocus(OtherActor);
		GetBlackboardComponent()->SetValueAsBool(BaseEnemyIsTargetInRangeKey, true);
	}

}

void ABaseEnemyController::EnemyOutOfRange(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == Cast<AActor>(TargetedActor) && GetBlackboardComponent())
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		GetBlackboardComponent()->ClearValue(BaseEnemyIsTargetInRangeKey);
	}
}

void ABaseEnemyController::EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(OtherActor);
	if(!AsBaseEntity || 
		GetControledEnemy()->GetRelationTowards(AsBaseEntity->GetTeamLabel()) != ETeamRelation::Hostile)
		return;

	// TODO: Aggro priority (?)
	if(TargetedActor) // already has a target
		return;

	TargetedActor = AsBaseEntity;
	GetBlackboardComponent()->SetValueAsObject(BaseEnemyTargetActorKey, Cast<ABaseEntity>(TargetedActor));
}

void ABaseEnemyController::ForgetEnemy(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(OtherActor);
	if(!AsBaseEntity)
		return;

	if(TargetedActor != AsBaseEntity)
		return;

	TArray<AActor *> EnemiesInRange = GetControledEnemy()->GetEnemiesInRange();
	if(EnemiesInRange.Num() == 0)
	{
		GetBlackboardComponent()->ClearValue(BaseEnemyTargetActorKey);
		TargetedActor = nullptr;
		return;
	}

}

float ABaseEnemyController::AttackCurrentTarget()
{
	return GetControledEnemy()->Attack(Cast<AActor>(TargetedActor));
}