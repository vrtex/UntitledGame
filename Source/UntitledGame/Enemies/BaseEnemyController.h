// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemies/BaseEnemy.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/TeamInterface.h"
#include "BaseEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	void SetupBehaviour();


public:

	virtual void Possess(APawn* aPawn) override;

	ABaseEnemy * GetControledEnemy() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		float AttackCurrentTarget();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		UBehaviorTree * BehaviourTreeOverride = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		UBlackboardData * UsedBlackboardOverride = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void EnemyInRange(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void EnemyOutOfRange(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void ForgetEnemy(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);


private:

	ABaseEnemy * Controlled = nullptr;

	ABaseEntity * TargetedActor = nullptr;



};
