// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Includes.h"
#include "ScalarResource.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UScalarResource : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScalarResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	float Get() const;

	float GetMax() const;

	void ChangeMax(float NewValue, bool bKeepRatio = true);

	float GetPercentage() const;

	float Remove(float ToRemove);

	float Restore(float ToRestore);

	void SetRegenRate(float NewRate);

	float GetRegenRate() const;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void TickRegen();

protected:

	float MaxValue = 100.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
		float RegenDelay = 0.1f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
		float CurrentValue = MaxValue;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float RegenRate = 1.f;

	FTimerHandle RegenTimer;

	bool bShouldRegen = true;
};
