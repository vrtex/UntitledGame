// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "Items/PickupItem.h"
#include "GameFramework/Character.h"
#include "Components/DetectionSphere.h"
#include "Items/ItemInfo.h"
#include "Interfaces/TeamInterface.h"
#include "Characters/Inventory.h"
#include "Components/CharacterStats.h"
#include "BaseEntity.h"
#include "ActiveSkills/SkillSet.h"
#include "BaseCharacter.generated.h"




UCLASS()
class UNTITLEDGAME_API ABaseCharacter : public ABaseEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Items")
		float GetPickupRange() const;

	UFUNCTION(BlueprintCallable, Category = "Items")
		float GetAttackRange() const;

	void SetAttackRange(float NewRange);

	void ChangeZoom(int32 Change);

	virtual bool DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator) override;

	bool UseSkill(ESkillSlot ToUse, ABaseEntity * Target, FVector TargetLocation);

	bool UseCurrentSkill();

	// bool UseSkill(UBaseSkill * ToUse, ABaseEntity * Target, FVector TargetLocation);

	UBaseSkill * GetSkill(ESkillSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		float Attack(AActor * Target);
	UFUNCTION(BlueprintCallable, Category = "Combat")
		TArray<AActor *> GetEnemiesInRange() const;

	UFUNCTION(BlueprintCallable, Category = "NPC")
		bool InteractWith(class ABaseNPC * NPC);
	UFUNCTION(BlueprintCallable, Category = "NPC")
		bool IsInteractableInRange(AActor * ToCheck) const;

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool Pickup(APickupItem * Target);
	UFUNCTION(BlueprintCallable, Category = "Items")
		TArray<AActor *> GetItemsInRange() const;

protected:


	UPROPERTY(EditAnywhere, Category = "Items")
		float ItemPickupRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "NPC")
		float InteractRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
		TSubclassOf<UBaseSkill> BaseAttackSkillClass;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent * CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent * Camera = nullptr;
public: // no fucks given
	UPROPERTY(VisibleAnywhere)
		UDetectionSphere * AttackRangeSphere = nullptr;
	UPROPERTY(VisibleAnywhere)
		UDetectionSphere * PickupRangeSphere = nullptr;
	UPROPERTY(VisibleAnywhere)
		UDetectionSphere * InteractRangeSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInventory * Backpack = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkillSet * SkillSet = nullptr;

private:

	ABaseEntity * TargetActor = nullptr;

	FVector TargetLocation;

	UBaseSkill * CurrentSkill = nullptr;

};
