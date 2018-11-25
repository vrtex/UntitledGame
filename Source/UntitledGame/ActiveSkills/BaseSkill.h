// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Includes.h"
#include "BaseSkill.generated.h"


UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	None,
	Primary,
	Secondary
};



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UBaseSkill : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// TargetLocation: mainly for skills not directed at specific entity
	UFUNCTION(BlueprintNativeEvent, Category = "Skills")
		bool Use(class ABaseEntity * User, class ABaseEntity * Target, FVector TargetLocation);
		
	virtual bool Use_Implementation(class ABaseEntity * User, class ABaseEntity * Target, FVector TargetLocation);

	UFUNCTION(BlueprintNativeEvent, Category = "Skills")
		void Cancel();

	virtual void Cancel_Implementation();

	UFUNCTION(BlueprintPure, Category = "Skills")
		UTexture2D * GetSkillIcon() const;

	UFUNCTION(BlueprintPure, Category = "Skills")
		float GetRange() const;

	UFUNCTION(BlueprintPure, Category = "Skills")
		float NeedsTarget() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		float Range = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		bool bNeedsTarget = true;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		UTexture2D * SkillIcon = nullptr;

	class ABaseEntity * CurrentUser = nullptr;


};
