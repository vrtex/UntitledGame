// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "Interfaces/DamageInfo.h"
#include "Components/CharacterStats.h"
#include "Interfaces/TeamInterface.h"
#include "ActiveSkills/BaseSkill.h"
#include "ActiveSkills/SkillSet.h"
#include "Components/DetectionSphere.h"
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

	UFUNCTION(BlueprintCallable, Category = "Combat")
		TArray<AActor *> GetEnemiesInRange() const;

	UFUNCTION(BlueprintPure, Category = "Items")
		float GetAttackRange() const;

	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetAttackRange(float NewRange);

	UFUNCTION(BlueprintCallable, Category = "General")
		void FaceActor(const AActor * Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		bool ReceiveDamageBP(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * DamageInstigator);

	bool ReceiveDamageBP_Implementation(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator);

	virtual bool ReceiveDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		bool DealDamageToBP(ABaseEntity * Target, const FDamageInfo & Damage, FDamageInfo & DealtDamage);

	bool DealDamageToBP_Implementation(ABaseEntity * Target, const FDamageInfo & Damage, FDamageInfo & DealtDamage);

	bool DealDamageTo(ABaseEntity * Target, const FDamageInfo & Damage, FDamageInfo & DealtDamage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		float RestoreHealthBP(const float ToRestore, ABaseEntity * Healer = nullptr);

	float RestoreHealthBP_Implementation(const float ToRestore, ABaseEntity * Healer = nullptr);

	float RestoreHealth(const float ToRestore, ABaseEntity * Healer = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		float RemoveManaBP(float ToRemove);

	float RemoveManaBP_Implementation(float ToRemove);

	float RemoveMana(float ToRemove);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		float RestoreManaBP(float ToRestore);

	float RestoreManaBP_Implementation(float ToRestore);

	float RestoreMana(float ToRestore);
	
	void OnStatsChange();

	virtual bool UseSkill(ESkillSlot ToUse, ABaseEntity * Target, FVector Location);

	UBaseSkill * GetSkill(ESkillSlot Slot);

	virtual void CancelCurrentSkill();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
		UCharacterStats * GetStats() const;

	float GetResistTo(EDamageElement Element, EDamageType Type) const;

	bool IsDead() const;

	// Resource getters: ---------------------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealthPercentage() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetMana() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetMaxMana() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetManaPercentage() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
		TSubclassOf<UBaseSkill> BaseAttackSkillClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCharacterStats * CharacterStats = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHealthComponent * Health = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UManaComponent * Mana = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkillSet * SkillSet = nullptr;

protected:
	UBaseSkill * CurrentSkill = nullptr;

	ABaseEntity * TargetActor = nullptr;

	FVector TargetLocation;


private:
	uint8 CurrentTeam;
	
	
};
