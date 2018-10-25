// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Includes.h"
#include "Interfaces/Interactable.h"
#include "Items/ItemInfo.h"
#include "PickupItem.generated.h"

UCLASS()
class UNTITLEDGAME_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SnapToGround();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Interact(AController * InstigatorController, APawn * InstigatorPawn);

	virtual void Hightlight(bool bTurnOn);

	FText GetInteractableName_Implementation() const;

	// call this every time an item is dropped
	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetHeldItem(FItemInfo NewItem);

	UFUNCTION(BlueprintPure, Category = "Items")
		FItemInfo GetHeldItem() const;

	// Bind this to timeline in BP
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Movement")
		void Bounce();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent * CollisionSphere = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent * ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		FItemInfo HeldItem;
};
