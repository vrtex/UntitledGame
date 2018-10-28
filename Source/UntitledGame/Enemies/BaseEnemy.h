// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interface/HealthBar.h"
#include "Interfaces/TeamInterface.h"
#include "Components/DetectionSphere.h"
#include "BaseEntity.h"
#include "BaseEnemy.generated.h"

UCLASS()
class UNTITLEDGAME_API ABaseEnemy : public ABaseEntity, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual bool Interact(AController *, APawn *) override;

	virtual bool Interact(AController * InstigatorController, APawn * InstigatorPawn) override;

	virtual void Hightlight(bool bTurnOn) override;

	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		virtual FText GetInteractableName() const;
	*/

	virtual FText GetInteractableName_Implementation() const;

	virtual bool DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "General")
		void FaceActor(AActor * Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float Attack(AActor * Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		TArray<AActor *> GetEnemiesInRange() const;

public: // fucks given: 0

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		UDetectionSphere * AttackRangeSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		float AttackRange = 100.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		UDetectionSphere * NoticeRangeSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		float NoticeRange = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		UDetectionSphere * ForgetRangeSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		float ForgetRange = 100.f;


protected:

	UPROPERTY(EditAnywhere, Category = "Combat|Setup")
		float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Setup|UI")
		FText DisplayName;

};
