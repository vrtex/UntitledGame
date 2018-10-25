// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"


void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// gimme dat juicy cursor
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	ClearMovementTarget();
	
	if(HealthBarClass.Get())
	{
		HealthBar = Cast<UHealthBar>(CreateWidget(this, HealthBarClass.Get(), FName("Health Bar Display")));
		HealthBar->AddToViewport();
	}
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HighlightUnderCursor();

	ShowHealthBar();

	if(TargetActor)
		MoveToActor(TargetActor);
	else if(CurrentMovementType != EMovementTargetType::None)
		FollowCursor();

	/*
	switch(CurrentMovementType)
	{
	case EMovementTargetType::None:
		break;
	case EMovementTargetType::Ground:
		FollowCursor();
		break;
	case EMovementTargetType::Item:
		TryPickupItem(Cast<APickupItem>(TargetActor));
		if(TargetActor)
			MoveToActor(TargetActor);
		break;
	case EMovementTargetType::Enemy:
		TryAttack(TargetActor);
		if(TargetActor)
			MoveToActor(TargetActor);
		break;
	default:
		break;
	}
	*/
}

void ABasePlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	ControlledCharacter = Cast<ABaseCharacter>(aPawn);
	if(!ensure(ControlledCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("No controlled character or wrong class of character"));
		FGenericPlatformMisc::RequestExit(false);
		return;
	}

	ControlledCharacter->AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayerController::EnemyDetected);
	ControlledCharacter->PickupRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayerController::ItemDetected);
	ControlledCharacter->InteractRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayerController::NPCDetected);

	InventoryWidget = Cast<UInventoryWidget>(CreateWidget(this, InventoryWidgetClass.Get(), FName("Inventory display")));
	InventoryWidget->SetupInventory(ControlledCharacter->Backpack);
}

void ABasePlayerController::OnLeftClick()
{
	FHitResult Hit;
	EMovementTargetType NewMoveType = GetIntendedTargetType(Hit);
	SetMovementTarget(NewMoveType, Hit.GetActor());
	if(CurrentInteractable && CurrentMovementType != EMovementTargetType::None)
	{
		CurrentInteractable->EndInteraction();
		CurrentInteractable = nullptr;
	}
	switch(CurrentMovementType)
	{
	case EMovementTargetType::None:
		break;
	case EMovementTargetType::Ground:
		break;
	case EMovementTargetType::Item:
		if(!GetControlledCharacter()->GetItemsInRange().Contains(TargetActor))
			break;
		ItemDetected(nullptr, TargetActor, nullptr, 0, false, FHitResult());
		// GetControlledCharacter()->Pickup(Cast<APickupItem>(TargetActor));
		ClearMovementTarget();
		break;
	case EMovementTargetType::Enemy:
		if(!GetControlledCharacter()->GetEnemiesInRange().Contains(TargetActor))
			break;
		GetControlledCharacter()->Attack(TargetActor);
		ClearMovementTarget();
		break;
	case EMovementTargetType::NPC:
		if(!GetControlledCharacter()->IsInteractableInRange(TargetActor))
			break;
		NPCDetected(nullptr, TargetActor, nullptr, 0, false, FHitResult());
		ClearMovementTarget();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Boo-Boo detected in Base player controller OnLeftClick() go fix"));
		break;
	}
}

void ABasePlayerController::OnLeftClickRelease()
{
	if(CurrentMovementType == EMovementTargetType::Ground)
		ClearMovementTarget();
}

void ABasePlayerController::ChangeCameraZoom(bool ZoomIn)
{
	if(ZoomIn)
		GetControlledCharacter()->ChangeZoom(-ZoomChangeSensitivity);
	else
		GetControlledCharacter()->ChangeZoom(ZoomChangeSensitivity);
}

void ABasePlayerController::ShowInventory()
{
	if(!IsValid(InventoryWidget))
	{
		return;

	}

	if(InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		return;
	}
	else
	{
		InventoryWidget->AddToViewport();
		return;
	}
}

void ABasePlayerController::OpenShop(UUserWidget * ShopWidget, ABaseNPC * ShopOwner)
{
	// TODO: shopping
}

ABaseCharacter * ABasePlayerController::GetControlledCharacter()
{
	return ControlledCharacter;
}

void ABasePlayerController::EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(CurrentMovementType != EMovementTargetType::Enemy)
		return;
	if(!IsValid(OtherActor) || OtherActor != TargetActor)
		return;

	GetControlledCharacter()->Attack(TargetActor);
	ClearMovementTarget();
}


void ABasePlayerController::ItemDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APickupItem * AsPickupItem = Cast<APickupItem>(OtherActor);
	bool bIsMoney = AsPickupItem->GetHeldItem().ItemType == EItemType::Money || AsPickupItem->GetHeldItem().ItemType == EItemType::RareMoney;
	if(CurrentMovementType != EMovementTargetType::Item && !bIsMoney)
		return;
	if(!AsPickupItem || !IsValid(AsPickupItem))
		return;
	if(!bIsMoney && OtherActor != TargetActor)
		return;
	GetControlledCharacter()->Pickup(AsPickupItem);
	if(OtherActor == TargetActor)
		ClearMovementTarget();
}

void ABasePlayerController::NPCDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABaseNPC * AsNPC = Cast<ABaseNPC>(OtherActor);
	if(!AsNPC)
		return;
	if(CurrentMovementType != EMovementTargetType::NPC || TargetActor != OtherActor)
		return;
	CurrentInteractable = AsNPC;
	ClearMovementTarget();
	GetControlledCharacter()->InteractWith(AsNPC);
	// interact if target actor is same as other actor
}

void ABasePlayerController::SetMovementTarget(EMovementTargetType MoveType, AActor * NewTargetActor)
{
	ClearMovementTarget();
	ITeamInterface * AsTeamAgent = nullptr;
	switch(MoveType)
	{
	case EMovementTargetType::None:
		TargetActor = nullptr;
		break;
	case EMovementTargetType::Ground:
		TargetActor = nullptr;
		break;
	case EMovementTargetType::Item:
		if(!Cast<APickupItem>(NewTargetActor))
		{
			ClearMovementTarget();
			return;
		}
		TargetActor = NewTargetActor;
		break;
	case EMovementTargetType::Enemy:
		AsTeamAgent = Cast<ITeamInterface>(NewTargetActor);
		if(!AsTeamAgent || 
			AsTeamAgent->GetRelationTowards(GetControlledCharacter()->GetTeamLabel()) != ETeamRelation::Hostile)
		{
			ClearMovementTarget();
			return;
		}

		TargetActor = NewTargetActor;
		break;
	case EMovementTargetType::NPC:
		if(!Cast<ABaseNPC>(NewTargetActor))
		{
			ClearMovementTarget();
			return;
		}
		TargetActor = NewTargetActor;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Boo-boo detected in base player controller. Function SetMovementTarget. Go fix"));
		break;
	}

	CurrentMovementType = MoveType;
}
	
EMovementTargetType ABasePlayerController::GetIntendedTargetType(FHitResult & HitUnderCursor)
{
	GetHitResultUnderCursor(CursorTraceChannel, false, HitUnderCursor);
	if(!HitUnderCursor.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clearing"));
		ClearMovementTarget();
		return EMovementTargetType::None;
	}

	if(Cast<ABaseNPC>(HitUnderCursor.GetActor()))
	{
		return EMovementTargetType::NPC;
	}

	if(Cast<ITeamInterface>(HitUnderCursor.GetActor()))
	{
		ITeamInterface * AsTeamAgent = Cast<ITeamInterface>(HitUnderCursor.GetActor());
		if(GetControlledCharacter()->GetRelationTowards(AsTeamAgent->GetTeamLabel()) == ETeamRelation::Hostile)
			return EMovementTargetType::Enemy;
		else
		{
			HitUnderCursor.ImpactPoint = HitUnderCursor.GetActor()->GetActorLocation();
			return EMovementTargetType::Ground;
		}
	}

	if(Cast<APickupItem>(HitUnderCursor.GetActor()))
	{
		return EMovementTargetType::Item;
	}

	return EMovementTargetType::Ground; // nothing of interest found, go to gorund location
}

bool ABasePlayerController::HighlightUnderCursor()
{
	if(FocusedIteractable)
	{
		FocusedIteractable->Hightlight(false);
		FocusedIteractable = nullptr;
	}
	FHitResult Hit;
	bool bGotHit = GetHitResultUnderCursor(CursorTraceChannel, false, Hit);
	if(!bGotHit)
		return false;
	FocusedIteractable = Cast<IInteractable>(Hit.GetActor());
	if(!FocusedIteractable)
		return false;
	FocusedIteractable->Hightlight(true);
	return true;
}

void ABasePlayerController::ShowHealthBar()
{
	ITeamInterface * AsEnemy = Cast<ITeamInterface>(FocusedIteractable);
	if(!AsEnemy || GetControlledCharacter()->GetRelationTowards(AsEnemy->GetTeamLabel()) != ETeamRelation::Hostile)
	{
		HealthBar->SetVisibility(ESlateVisibility::Hidden);
		HealthBar->SetOwningEnemy(nullptr);
		return;
	}

	HealthBar->SetOwningEnemy(Cast<AActor>(FocusedIteractable));
	HealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ABasePlayerController::ClearMovementTarget()
{
	CurrentMovementType = EMovementTargetType::None;
	TargetActor = nullptr;
}

void ABasePlayerController::FollowCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(CursorTraceChannel, false, Hit);
	if(!Hit.bBlockingHit)
		return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
}

void ABasePlayerController::MoveToActor(AActor * Target)
{
	UAIBlueprintHelperLibrary::SimpleMoveToActor(this, Target);
}

void ABasePlayerController::TryPickupItem(APickupItem * ToPickup)
{
	if(!IsValid(ToPickup))
		return;
	if(FVector::Dist(ToPickup->GetActorLocation(), GetControlledCharacter()->GetActorLocation()) < ControlledCharacter->GetPickupRange())
	{
		ClearMovementTarget();
		ToPickup->Destroy();
		return;
	}
}

float ABasePlayerController::TryAttack(AActor * Target)
{
	if(!IsValid(Target))
		return 0.f;

	if(FVector::Dist(Target->GetActorLocation(), GetControlledCharacter()->GetActorLocation()) < ControlledCharacter->GetAttackRange())
	{
		ClearMovementTarget();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetControlledCharacter()->GetActorLocation(),
			Target->GetActorLocation()
		);
		GetControlledCharacter()->SetActorRotation(TargetRotation);
		float DealtDamage = UGameplayStatics::ApplyDamage(Target, 10.f, this, GetControlledCharacter(), UDamageType::StaticClass());
		return DealtDamage;
	}
	return 0.f;
}