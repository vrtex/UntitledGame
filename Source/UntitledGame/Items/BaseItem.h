// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/ItemInfo.h"
#include "BaseItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNTITLEDGAME_API UBaseItem : public UObject
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintImplementableEvent)
		FItemInfo Get();
	
	
};
