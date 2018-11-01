// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "Interfaces/DamageInfo.h"
#include "Components/CharacterStats.h"
#include "Interfaces/TeamInterface.h"
#include "Includes.h"
#include "BaseEntity.generated.h"

UCLASS()
class UNTITLEDGAME_API ABaseEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEntity();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetTeamLabel(uint8 OtherLabel);

	virtual uint8 GetTeamLabel() const;

	virtual ETeamRelation GetRelationTowards(uint8 OtherLabel) const;

	UFUNCTION(BlueprintCallable, Category = "General")
		void FaceActor(const AActor * Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		bool DealDamageBP(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * DamageInstigator);

	bool DealDamageBP_Implementation(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator);

	virtual bool DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator);
	
	float GetResistTo(EDamageElement Element, EDamageType Type) const;

	bool IsDead() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealthPercentage() const;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCharacterStats * CharacterStats = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHealthComponent * Health = nullptr;

private:
	uint8 CurrentTeam;
	
	
};
