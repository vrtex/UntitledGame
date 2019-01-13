// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "Components/ActorComponent.h"
#include "CharacterLevel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UCharacterLevel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterLevel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	UFUNCTION(BlueprintCallable, Category = "Experience")
		void AddXP(float ToAdd);

	UFUNCTION(BlueprintCallable, Category = "Experience")
		void LevelUp();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Experience")
		float GetCurrentXP() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Experience")
		float GetMaxXP() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Experience")
		float GetCurrentPercentage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Experience")
		int32 GetCurrentLevel() const;

	FLevelDelegate OnLevelUp;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
		int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
		float CurrentXP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
		float MaxXP = 100.f;

private:
	UParticleSystem * LevelUpParticles = nullptr;
	
};
