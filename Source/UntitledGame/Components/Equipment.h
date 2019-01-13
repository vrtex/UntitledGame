// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Includes.h"
#include "Items/ItemInfo.h"
#include "Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UEquipment : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipment();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttachStats(class UCharacterStats * S);

	void AttachInventory(class UInventory * Inv);

	FEquipmentDelegate OnChange;

	// Returns item that was previously equppied
	UFUNCTION(BlueprintCallable, Category = "Items")
		FItemInfo Equip(const FItemInfo & Item);

	// Returns removed item (or empty item of none was equipped)
	UFUNCTION(BlueprintCallable, Category = "Items")
		FItemInfo Unequip(EItemType Slot);

	UFUNCTION(BlueprintCallable, Category = "Items")
		void SendToBackpack(const FItemInfo & Item);

	void ManipulateStats(const FItemInfo & Item, bool bEquip);

	UFUNCTION(BlueprintPure, Category = "Items")
		bool HasItem(const EItemType slot) const;

	UFUNCTION(BlueprintPure, Category = "Items")
		FItemInfo GetItem(const EItemType Slot) const;

private:
	TMap<EItemType, FItemInfo> CurrentEquipment;

protected:
	class UCharacterStats * ManipulatedStats = nullptr;

	class UInventory * Backpack = nullptr;
};
