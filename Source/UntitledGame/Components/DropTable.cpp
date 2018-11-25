// Fill out your copyright notice in the Description page of Project Settings.

#include "DropTable.h"


// Sets default values for this component's properties
UDropTable::UDropTable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDropTable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	ItemsWeight = 0.f;

	for(auto & Item : Items)
	{
		Item.Min = ItemsWeight;
		ItemsWeight += Item.GetChance();
		Item.Chance += Item.Min;
	}

}


// Called every frame
void UDropTable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDropTable::FillInventory(UInventory * ToFill)
{
	if(!ToFill)
		return;

	TArray<float> Rolls;
	for(int32 i = 0; i < MaxItems; ++i)
		Rolls.Add(FMath::RandRange(0.f, ItemsWeight));

	for(float Roll : Rolls)
	{
		UBaseItem * ItemCreator = nullptr;
		for(FDropChance & Item : Items)
		{
			if(Item.InRange(Roll))
			{
				TSubclassOf<UBaseItem> ItemClass;
				ItemClass = Item.Item.Get();
				if(!ItemClass || !ItemClass.Get())
					break;
				ItemCreator = Cast<UBaseItem>(NewObject<UBaseItem>(GetOwner(), ItemClass.Get()));
				break;
			}
		}

		if(ItemCreator)
		{
			ToFill->AddItem(ItemCreator->Get());
		}
	}
}