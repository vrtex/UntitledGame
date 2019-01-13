// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/StatsModifier.h"
#include "Includes.h"
#include "SkillTreeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Setup(class ABaseCharacter * Owner);
	void Setup_Implementation(class ABaseCharacter * Owner);
	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NotifyCharacter(class USkillTreeNode * Node, bool bAdding);

protected:
	UFUNCTION(BlueprintCallable)
		void CrawlNodes();

	UPROPERTY(BlueprintReadOnly)
		TArray<class USkillTreeNode *> Nodes;

	UPROPERTY(BlueprintReadOnly)
		class ABaseCharacter * Owner;
};

UCLASS()
class UNTITLEDGAME_API USkillTreeNode : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Setup(USkillTreeWidget * Owner);
	void Setup_Implementation(USkillTreeWidget * Owner);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		FStatsModifierList Mods;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		UTexture2D * NodeIcon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkillTreeWidget * Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<USkillTreeNode *> NextNodes;
};