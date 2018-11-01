// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Includes.h"
#include "Items/PickupItem.h"
#include "BaseCharacter.h"
#include "Interfaces/Interactable.h"
#include "Interface/HealthBar.h"
#include "Interface/InventoryWidget.h"
#include "NPC/BaseNPC.h"
#include "ActiveSkills/BaseSkill.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMovementTargetType : uint8
{
	None,
	Ground, // only one allowing for click and hold movement
	Item,
	Enemy,
	NPC
};

UCLASS()
class UNTITLEDGAME_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable, Category = "Input")
		void OnLeftClick();
	UFUNCTION(BlueprintCallable, Category = "Input")
		void OnLeftClickRelease();
	UFUNCTION(BlueprintCallable, Category = "Input")
		void UseAlternateSkill();
	UFUNCTION(BlueprintCallable, Category = "Input")
		void ChangeCameraZoom(bool ZoomIn);
	UFUNCTION(BlueprintCallable, Category = "Input")
		void ShowInventory();
	UFUNCTION(BlueprintCallable, Category = "Input")
		void OpenShop(UUserWidget * ShopWidget, ABaseNPC * ShopOwner);

	UFUNCTION(BlueprintCallable, Category = "Character")
		ABaseCharacter * GetControlledCharacter();

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void ItemDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void NPCDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


protected:
	UPROPERTY(EditAnywhere, Category = "Setup")
		int32 ZoomChangeSensitivity = 50;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		UHealthBar * HealthBar = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UHealthBar> HealthBarClass;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UInventoryWidget * InventoryWidget = nullptr;

	void SetMovementTarget(EMovementTargetType MoveType, AActor * NewTargetActor);


private:

	ECollisionChannel CursorTraceChannel = ECollisionChannel::ECC_CursorTrace;

	EMovementTargetType GetIntendedTargetType(FHitResult & HitUnderCursor);

	EMovementTargetType CurrentMovementType;

	AActor * TargetActor = nullptr;

	//UBaseSkill * CurrentSkill = nullptr;

	ABaseCharacter * ControlledCharacter = nullptr;

	bool bFollowCursor = false;

	bool HighlightUnderCursor();

	void ShowHealthBar();

	void ClearMovementTarget();

	void FollowCursor();

	void MoveToActor(AActor * Target);

	void TryPickupItem(APickupItem * ToPickup);

	// float TryAttack(AActor * Target);

	//APickupItem * TargetItem = nullptr;
	
	IInteractable * FocusedIteractable = nullptr;

	IInteractable * CurrentInteractable = nullptr;
};
