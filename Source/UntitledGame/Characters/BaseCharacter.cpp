// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "NPC/BaseNPC.h"
#include "Items/PickupItem.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionProfileName(PlayerCollisionPreset);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bAbsoluteLocation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CameraBoom);

	AttackRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Attack Range Sphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);
	AttackRangeSphere->SetSphereRadius(AttackRange);

	PickupRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Pickup Range Sphere"));
	PickupRangeSphere->SetupAttachment(RootComponent);
	PickupRangeSphere->SetSphereRadius(ItemPickupRange);

	InteractRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Interact Range Sphere"));
	InteractRangeSphere->SetupAttachment(RootComponent);
	InteractRangeSphere->SetSphereRadius(InteractRange);

	CharacterStats = CreateDefaultSubobject<UCharacterStats>(FName("Character Stats"));
	
	Backpack = CreateDefaultSubobject<UInventory>(FName("Backpack"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	AttackRangeSphere->SetSphereRadius(AttackRange);
	PickupRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Item);
	PickupRangeSphere->SetSphereRadius(ItemPickupRange);
	InteractRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_NPC);
	InteractRangeSphere->SetSphereRadius(InteractRange);

	SetTeamLabel(1);


}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseCharacter::GetPickupRange() const
{
	return ItemPickupRange;
}

float ABaseCharacter::GetAttackRange() const
{
	return AttackRangeSphere->GetScaledSphereRadius();
}

void ABaseCharacter::ChangeZoom(int32 Change)
{
	float NewLength = FMath::Clamp<float>(CameraBoom->TargetArmLength + (float)Change, 300, 1000);
	CameraBoom->TargetArmLength = NewLength;
}


void ABaseCharacter::SetTeamLabel(uint8 NewLabel)
{
	CurrentTeam = NewLabel;
}

uint8 ABaseCharacter::GetTeamLabel() const
{
	return CurrentTeam;
}

bool ABaseCharacter::DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ACharacter * DamageDealer, AController * Instigator)
{
	for(TPair<EDamageElement, float> PartialDamage : Damage.PhysicalDamage)
	{
		float ThisElement = Damage.PhysicalDamage[PartialDamage.Key] * (1.f - GetResistTo(PartialDamage.Key, true));
		DealtDamage.PhysicalDamage[PartialDamage.Key] = ThisElement;
	}

	for(TPair<EDamageElement, float> PartialDamage : Damage.MagicDamage)
	{
		float ThisElement = Damage.MagicDamage[PartialDamage.Key] * (1.f - GetResistTo(PartialDamage.Key, false));
		DealtDamage.MagicDamage[PartialDamage.Key] = ThisElement;
	}
	UE_LOG(LogTemp, Warning, TEXT("%f: Taking %f damage"), GetWorld()->GetTimeSeconds(), DealtDamage.GetTotalDamage());

	return !FMath::IsNearlyEqual(0, DealtDamage.GetTotalDamage());
}

void ABaseCharacter::FaceActor(AActor * Target)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		Target->GetActorLocation()
	);
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;
	SetActorRotation(TargetRotation);
}

float ABaseCharacter::Attack(AActor * Target)
{
	if(!IsValid(Target))
		return 0.f;
	ITeamInterface * AsTeamTarget = Cast<ITeamInterface>(Target);
	if(!AsTeamTarget || AsTeamTarget->GetRelationTowards(GetTeamLabel()) != ETeamRelation::Hostile)
	{
		return 0.f;
	}
	GetCharacterMovement()->StopMovementImmediately();
	FaceActor(Target);
	FDamageInfo DamageInfo;
	DamageInfo.PhysicalDamage.Add(EDamageElement::None, 10.f);
	FDamageInfo DealtDamageInfo;

	float ActualDamage = AsTeamTarget->DealDamage(DamageInfo, DealtDamageInfo, this, GetController());
	return DealtDamageInfo.GetTotalDamage();
}

bool ABaseCharacter::Pickup(APickupItem * Target)
{
	APickupItem * AsPickupItem = Cast<APickupItem>(Target);
	if(!AsPickupItem)
		return false;
	switch(Target->GetHeldItem().ItemType)
	{
	case EItemType::Money:
	case EItemType::RareMoney:
		Backpack->AddMoney(Target->GetHeldItem());
		Target->Destroy();
		return true;
	default:
		break;
	}
	GetCharacterMovement()->StopMovementImmediately();

	if(Backpack->AddItem(AsPickupItem->GetHeldItem()))
	{
		Target->Destroy();
		return true;
	}
	AsPickupItem->Bounce();
	return true;
}

bool ABaseCharacter::InteractWith(ABaseNPC * Target)
{
	ABaseNPC * NPC = Cast<ABaseNPC>(Target);
	if(!NPC)
		return false;
	GetCharacterMovement()->StopMovementImmediately();
	FaceActor(NPC);
	NPC->Interact(GetController(), this);
	return true;
}

TArray<AActor*> ABaseCharacter::GetEnemiesInRange() const
{
	TArray<AActor*> Enemies;
	AttackRangeSphere->GetOverlappingActors(Enemies);
	return Enemies;
}

float ABaseCharacter::GetResistTo(EDamageElement Element, bool bPhysical) const
{
	return CharacterStats->GetResistTo(Element);
}

TArray<AActor*> ABaseCharacter::GetItemsInRange() const
{
	TArray<AActor*> Items;
	PickupRangeSphere->GetOverlappingActors(Items);
	return Items;
}

bool ABaseCharacter::IsInteractableInRange(AActor * ToCheck) const
{
	return InteractRangeSphere->IsOverlappingActor(Cast<AActor>(ToCheck)) && Cast<IInteractable>(ToCheck);
}