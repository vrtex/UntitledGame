// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseNPC.h"


// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionProfileName(NPCCollisionPreset);
	
	ShopContent = CreateDefaultSubobject<UInventory>(FName("Shop Content"));
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
	if(ShopWidgetClass.Get())
		ShopWidget = Cast<UShopWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), ShopWidgetClass.Get()));
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseNPC::Interact(AController * InstigatorController, APawn * InstigatorPawn)
{
	// TODO: khajit
	UE_LOG(LogTemp, Warning, TEXT("Khajit has wares if you have a coin"));
	ShopContent->PrintItems();
	OpenShopDelegate.Broadcast(ShopWidget, this);
	ShowShopWidget();
	return false;
}

void ABaseNPC::EndInteraction()
{
	ShowShopWidget(false);
}

void ABaseNPC::Hightlight(bool bTurnOn)
{
	GetMesh()->SetRenderCustomDepth(bTurnOn);
}

void ABaseNPC::ShowShopWidget(bool bShow)
{
	if(!ShopWidget)
	{

		UE_LOG(LogTemp, Warning, TEXT("no shop"));
		return;
	}
	if(bShow)
		ShopWidget->AddToViewport();
	else
		ShopWidget->RemoveFromParent();

}