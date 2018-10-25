// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "Interactable.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class UNTITLEDGAME_API UInteractable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class UNTITLEDGAME_API IInteractable
{
	GENERATED_IINTERFACE_BODY()
public:

	virtual bool Interact(AController * InstigatorController, APawn * InstigatorPawn) = 0;

	virtual void EndInteraction() {};

	virtual void Hightlight(bool bTurnOn) {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FText GetInteractableName() const;
};