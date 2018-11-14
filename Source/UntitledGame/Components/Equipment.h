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

	void AttackStats(class UCharacterStats * S);

	FEquipmentDelegate OnChange;

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool Equip(const FItemInfo & Item);

	UFUNCTION(BlueprintCallable, Category = "Items")
		bool Unequip(EItemType Slot);

	UFUNCTION(BlueprintPure, Category = "Items")
		FItemInfo GetItem(const EItemType Slot) const;

private:
	TMap<EItemType, FItemInfo> CurrentEquipment;

protected:
	class UCharacterStats * ManipulatedStats = nullptr;
};
