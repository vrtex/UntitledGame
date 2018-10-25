// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Includes.h"
#include "Interfaces/DamageInfo.h"
#include "CharacterStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	UFUNCTION(BlueprintPure, Category = "Combat")
		float GetResistTo(EDamageElement Element) const;
protected:

	// Clamp those bitches to <0, 1> or do some funky shit
	UPROPERTY(EditAnywhere, Category = "Defence|Resists")
		TMap<EDamageElement, float> ElementalResists;
};
