// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
* IMPORTANT: only include in .cpp
* forward declare only as last resort
**/

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Includes.h"
#include "Items/PickupItem.h"
#include "UntitledGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API AUntitledGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		TSubclassOf<APickupItem> PickupItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		UBlackboardData * BaseEnemyBlackboard;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		UBehaviorTree * BaseEnemyBehaviourTree;

};
