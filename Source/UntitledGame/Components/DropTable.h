// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/BaseItem.h"
#include "Characters/Inventory.h"
#include "DropTable.generated.h"



USTRUCT(BlueprintType, Blueprintable)
struct FDropChance
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UBaseItem> Item;

	float Min = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Chance = 0.f;


	float GetChance() const { return Chance - Min; };

	bool InRange(float Roll) { return Roll >= Min && Roll < Chance; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDGAME_API UDropTable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDropTable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FillInventory(UInventory * ToFill);
protected:
	// Only change max in setup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		TArray<FDropChance> Items;

	UPROPERTY(EditAnywhere, Category = "Setup")
		int32 MaxItems = 1;

private:

	float ItemsWeight = 0;
};
