// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Includes.h"
#include "Interfaces/DamageInfo.h"
#include "Interfaces/StatsModifier.h"
#include "Components/CharacterStat.h"
#include "CharacterStats.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatsDelegate);


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

	FStatsDelegate OnChange;

	UFUNCTION()
		void AddLevel();

	/*
	void AddMod(const FStatsModifier & Mod);

	void RemoveMod(const FStatsModifier & Mod);
	*/

	UFUNCTION(BlueprintCallable)
		void AddList(const FStatsModifierList & List);

	UFUNCTION(BlueprintCallable)
		void RemoveList(const FStatsModifierList & List);

	UFUNCTION(BlueprintPure)
		UCharacterStat * GetStat(EStatField Field) const;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetMaxHealth(float NewMax);

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
		void SetMaxMana(float NewMax);

	UFUNCTION(BlueprintPure, Category = "Mana")
		float GetMaxMana() const;

	UFUNCTION(BlueprintPure, Category = "Defence|Resists")
		float GetResistTo(EDamageElement Element) const;

	UFUNCTION(BlueprintCallable, Category = "Defence|Resists")
		void SetResistTo(EDamageElement Element, float NewValue);
	
protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TMap<EStatField, UCharacterStat *> StatList;

	// Clamp those bitches to <0, 1> or do some funky shit
	UPROPERTY(EditAnywhere, Category = "Defence|Resists")
		TMap<EDamageElement, float> ElementalResists;


};
