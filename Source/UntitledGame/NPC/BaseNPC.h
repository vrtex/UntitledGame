// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Includes.h"
#include "Interfaces/Interactable.h"
#include "Characters/BaseCharacter.h"
#include "Interface/ShopWidget.h"
#include "BaseNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBaseNPCDelegate, UUserWidget *, ShopWidget, ABaseNPC *, ShopOwner);

UCLASS()
class UNTITLEDGAME_API ABaseNPC : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool Interact(AController * InstigatorController, APawn * InstigatorPawn) override;

	virtual void EndInteraction() override;

	virtual void Hightlight(bool bTurnOn);

	// TODO: move these to shopkeep subclass
	FBaseNPCDelegate OpenShopDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Items")
		TSubclassOf<UShopWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
		UInventory * ShopContent = nullptr;
private:
	UShopWidget * ShopWidget = nullptr;

	void ShowShopWidget(bool bShow = true);

};
