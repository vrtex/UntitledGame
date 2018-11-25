// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "UntitledGameGameModeBase.h"


// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;



	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...

	SetPickupItemClass();
	EmptyInventory();
	
}

bool UInventory::AddItem(FItemInfo ToAdd)
{
	if(ToAdd.bStackable)
	{
		/*
		// TODO: check if item of same class is already added
			// if true check of StackSize < MaxStackSize
				// if true add to stack
		*/
		bool bGotItem = false;
		int32 i = 0;
		for(i = 0; i < GetCapacity(); ++i)
		{
			if(Content[i].ItemType == ToAdd.ItemType) //&& Content[i].StackSize < Content[i].MaxStackSize)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found place"));
				bGotItem = true;
				break;
			}
		}
		if(bGotItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("adding to stack"));
			Content[i].StackSize++;
			OnPickup.Broadcast(this, true, ToAdd);
			return true;
		}
	}

	if(GetNumberOfItems() >= GetCapacity())
		return false;
	int32 EmptyIndex = GetFirstEmptySlot();
	if(EmptyIndex < 0)
		return false;
	// Content[GetNumberOfItems()] = ToAdd;
	Content[EmptyIndex] = ToAdd;
	CurrentSize = CurrentSize + 1;
	OnPickup.Broadcast(this, true, ToAdd);
	return true;
}

bool UInventory::AddForced(FItemInfo ToAdd)
{
	if(AddItem(ToAdd))
		return true;

	DropForced(ToAdd);
	return false;
}

bool UInventory::RemoveItemAtIndex(int32 Index)
{
	if(Content[Index].ItemType == EItemType::None)
		return false;
	// if item at Index is sackable check is stacks size > 1
		// if true remove 1 from stack and return true


	FItemInfo EmptyItem;
	FItemInfo Removed = Content[Index];
	Content[Index] = EmptyItem;
	CurrentSize = CurrentSize - 1;
	OnDrop.Broadcast(this, false, Removed);
	return true;
}

bool UInventory::RemoveItemAtIndexStack(int32 Index, int32 Count)
{
	if(GetNumberOfItems() <= Index || !Content[Index].bStackable || Content[Index].StackSize < Count)
		return false;

	for(int32 i = 0; i < Count; ++i)
	{
		RemoveItemAtIndex(Index);
	}

	return true;
}

void UInventory::DropForced(FItemInfo & ToDrop)
{
	AActor * Owner = GetOwner();
	if(!Owner || !DroppedItemClass.Get())
		return;

	APickupItem * DroppedItem = Owner->GetWorld()->SpawnActorDeferred<APickupItem>(DroppedItemClass.Get(), Owner->GetTransform());
	DroppedItem->SetHeldItem(ToDrop);

	FTransform DropTransform = Owner->GetTransform();
	DropTransform.AddToTranslation(
		FVector(FMath::RandRange(-DropRange, DropRange), FMath::RandRange(-DropRange, DropRange), 0));

	UGameplayStatics::FinishSpawningActor(DroppedItem, DropTransform);
}

bool UInventory::DropItem(int32 Index)
{
	if(!HasItemAtIndex(Index))
		return false;
	FItemInfo ToDrop = GetItem(Index);
	RemoveItemAtIndex(Index);
	AActor * Owner = GetOwner();
	if(!Owner || !DroppedItemClass.Get())
		return false;
	DropForced(ToDrop);
	return true;
}

void UInventory::DropAll()
{
	for(int32 i = 0; i < GetCapacity(); ++i)
		DropItem(i);
}

void UInventory::SwapItems(int32 Index, int32 OtherIndex)
{
	if(Index >= GetCapacity() || OtherIndex >= GetCapacity())
		return;

	FItemInfo temp = Content[Index];
	Content[Index] = Content[OtherIndex];
	Content[OtherIndex] = temp;
	OnPickup.Broadcast(this, false, FItemInfo());
}

void UInventory::PrintItems()
{
	UE_LOG(LogTemp, Warning, TEXT("Size:, %i"), Content.Num());
	for(int32 i = 0; i < Content.Num(); ++i)
	{
		FText Name = GetItem(i).ItemName;
		UE_LOG(LogTemp, Warning, TEXT("Name, %s"), *(Name.ToString()));
	}
}

int32 UInventory::GetFirstEmptySlot() const
{
	int32 i;
	for(i = 0; i < GetCapacity(); ++i)
	{
		if(!HasItemAtIndex(i))
			return i;
	}
	return -1;
}

FItemInfo UInventory::GetItem(int32 Index) const
{
	if(Index >= GetCapacity())
		Index = GetCapacity() - 1;

	if(Index < 0)
		Index = 0;
	//FItemInfo ToReturn = Content[Index];
	return Content[Index];
}

bool UInventory::AddMoney(FItemInfo Money)
{
	switch(Money.ItemType)
	{
	case EItemType::Money:
		CurrentMoney += Money.StackSize;
		OnPickup.Broadcast(this, true, Money);
		return true;
	case EItemType::RareMoney:
		CurrentRareMoney += Money.StackSize;
		OnPickup.Broadcast(this, true, Money);
		return true;
	default:
		break;
	}
	return false;
}

float UInventory::GetMoneyCount() const
{
	return CurrentMoney;
}

float UInventory::GetRareMoneyCount() const
{
	return CurrentRareMoney;
}

bool UInventory::HasEmptyPlace(FItemInfo & Stacking) const
{
	if(!Stacking.bStackable)
		return GetFirstEmptySlot() != -1;

	for(int32 i = 0; i < Content.Num(); ++i)
	{
		if(Content[i].ItemType == Stacking.ItemType && Content[i].StackSize < Content[i].MaxStackSize)
			return true;
	}
	return false;
}


bool UInventory::HasItemAtIndex(int32 Index) const
{
	return GetItem(Index).ItemType != EItemType::None;
}

void UInventory::EmptyInventory()
{
	Content.Empty();
	Content.SetNum(Capacity);
	/*
	for(int32 i = 0; i < GetCapacity(); ++i)
	{
		FItemInfo EmptyItem;
		Content.Add(EmptyItem);
	}
	*/
	CurrentSize = 0;

	// OnDrop.Broadcast(this, false, FItemInfo());
}

int32 UInventory::GetNumberOfItems() const
{
	return CurrentSize;
}

int32 UInventory::SetCapacity(int32 NewSize)
{
	Capacity = NewSize;
	EmptyInventory();
	return GetCapacity();
}

int32 UInventory::GetCapacity() const
{
	return Capacity;
}

ACharacter * UInventory::GetInventoryOwner() const
{
	AActor * Owner = GetOwner();
	ACharacter * Character = Cast<ACharacter>(Owner);
	return nullptr;//Cast<ACharacter>(Owner) ? Cast<ACharacter>(Owner) : nullptr;
}

void UInventory::SetPickupItemClass()
{

	AUntitledGameGameModeBase * GameMode = Cast<AUntitledGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory at actor %s has no dropped item class set"), *GetOwner()->GetName());
		return;
	}

	DroppedItemClass = GameMode->PickupItemClass;

	if(!DroppedItemClass.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory at actor %s has no dropped item class set"), *GetOwner()->GetName());
		return;
	}
}

