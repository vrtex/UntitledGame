// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Includes.h"
#include "Characters/BaseCharacter.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSkillbar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void Setup(ABaseCharacter * Character);
	
	void Setup_Implementation(ABaseCharacter * Character);

protected:
	UPROPERTY(BlueprintReadOnly)
		ABaseCharacter * PlayerChar = nullptr;
};
