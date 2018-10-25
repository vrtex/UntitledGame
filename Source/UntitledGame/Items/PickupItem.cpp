// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"


// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Pickup collision"));
	SetRootComponent(CollisionSphere);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Item Display Mesh"));
	ItemMesh->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionProfileName(ItemCollisionPreset);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	/*
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_CursorTrace, ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Block);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_Item);
	*/


	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SnapToGround();
	Bounce();
}

void APickupItem::SnapToGround()
{
	FHitResult Hit;
	FVector EndTraceLocation = GetActorLocation() + GetActorUpVector() * -1000;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool bFoundGround =
		GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), EndTraceLocation, ECollisionChannel::ECC_Ground, QueryParams);
	if(!bFoundGround)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ground found"));
		return;
	}
	SetActorLocation(Hit.ImpactPoint);
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickupItem::Interact(AController * InstigatorController, APawn * InstigatorPawn)
{
	return false;
}

void APickupItem::Hightlight(bool bTurnOn)
{
	ItemMesh->SetRenderCustomDepth(bTurnOn);
}

FText APickupItem::GetInteractableName_Implementation() const
{
	return HeldItem.ItemName;
}

void APickupItem::SetHeldItem(FItemInfo NewItem)
{
	HeldItem = NewItem;
}

FItemInfo APickupItem::GetHeldItem() const
{
	return HeldItem;
}