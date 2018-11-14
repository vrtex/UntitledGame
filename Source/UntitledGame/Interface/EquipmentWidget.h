// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/BaseCharacter.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Equipment")
		void UpdateEquipment();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
		void Setup(class ABaseCharacter * Character);

	void Setup_Implementation(class ABaseCharacter * Character);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		ABaseCharacter * Owner;
	
};
