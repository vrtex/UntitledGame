// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interface/HealthBar.h"
#include "Interfaces/TeamInterface.h"
#include "Components/DetectionSphere.h"
#include "BaseEnemy.generated.h"

UCLASS()
class UNTITLEDGAME_API ABaseEnemy : public ACharacter, public IInteractable, public ITeamInterface
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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetTeamLabel(uint8 Label) override;

	virtual uint8 GetTeamLabel() const override;

	virtual bool DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ACharacter * DamageDealer, AController * Instigator) override;

	// TODO: Move these to TeamInterface
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float GetHealthPercentage() const;
	UFUNCTION(BlueprintCallable, Category = "General")
		void FaceActor(AActor * Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
		float Attack(AActor * Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		TArray<AActor *> GetEnemiesInRange() const;

public:

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

	uint8 CurrentTeam;
};
