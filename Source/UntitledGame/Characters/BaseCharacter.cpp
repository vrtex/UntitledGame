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
	CameraBoom->SetRelativeRotation(FRotator(-70.f, 0, 0));
	CameraBoom->TargetArmLength = 800.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CameraBoom);

	PickupRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Pickup Range Sphere"));
	PickupRangeSphere->SetupAttachment(RootComponent);
	PickupRangeSphere->SetSphereRadius(ItemPickupRange);

	InteractRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Interact Range Sphere"));
	InteractRangeSphere->SetupAttachment(RootComponent);
	InteractRangeSphere->SetSphereRadius(InteractRange);

	
	Backpack = CreateDefaultSubobject<UInventory>(FName("Backpack"));
	Equipment = CreateDefaultSubobject<UEquipment>(FName("Equipment"));
	// TODO: component not created
	LevelComponent = CreateDefaultSubobject<UCharacterLevel>(FName("LevelComponent"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	PickupRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Item);
	PickupRangeSphere->SetSphereRadius(ItemPickupRange);
	InteractRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_NPC);
	InteractRangeSphere->SetSphereRadius(InteractRange);

	Equipment->AttachInventory(Backpack);
	Equipment->AttachStats(GetStats());

	SkillSet->AttachEquipment(Equipment);
	if(LevelComponent)
		LevelComponent->OnLevelUp.AddDynamic(GetStats(), &UCharacterStats::AddLevel);
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DUPA2"));
	}

	SetTeamLabel(GenericPlayerTeam);
}

void ABaseCharacter::RecreateBubles()
{
	if(!PickupRangeSphere)
	{
		PickupRangeSphere = NewObject<UDetectionSphere>(this, UDetectionSphere::StaticClass(), FName("Pickup Range Sphere"));
		PickupRangeSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		PickupRangeSphere->SetSphereRadius(ItemPickupRange);
	}

	if(!InteractRangeSphere)
	{
		InteractRangeSphere = NewObject<UDetectionSphere>(this, UDetectionSphere::StaticClass(), FName("Interact Range Sphere"));
		InteractRangeSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		InteractRangeSphere->SetSphereRadius(InteractRange);
	}
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

UParticleSystem * ABaseCharacter::GetLevelUpParticles() const
{
	return LevelUpParticles;
}

void ABaseCharacter::ChangeZoom(int32 Change)
{
	float NewLength = FMath::Clamp<float>(CameraBoom->TargetArmLength + (float)Change, 300, 1000);
	CameraBoom->TargetArmLength = NewLength;
}


bool ABaseCharacter::ReceiveDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
{
	float TotalDamage = Super::ReceiveDamage(Damage, DealtDamage, DamageDealer, Instigator);
	return TotalDamage;
}

bool ABaseCharacter::UseCurrentSkill()
{
	if(!CurrentSkill)
		return false;
	return CurrentSkill->Use(this, TargetActor, TargetLocation);
}

float ABaseCharacter::Attack(AActor * Target)
{
	if(!IsValid(Target))
		return 0.f;
	ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(Target);
	if(!AsBaseEntity || AsBaseEntity->GetRelationTowards(GetTeamLabel()) != ETeamRelation::Hostile)
	{
		return 0.f;
	}
	GetCharacterMovement()->StopMovementImmediately();
	FaceActor(Target);
	FDamageInfo DamageInfo;
	DamageInfo.PhysicalDamage.Add(EDamageElement::None, 10.f);
	FDamageInfo DealtDamageInfo;
	// float ActualDamage = AsBaseEntity->ReceiveDamage(DamageInfo, DealtDamageInfo, this, GetController());
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

TArray<AActor*> ABaseCharacter::GetItemsInRange() const
{
	TArray<AActor*> Items;
	PickupRangeSphere->GetOverlappingActors(Items);
	return Items;
}

UEquipment * ABaseCharacter::GetEquipment() const
{
	return Equipment;
}

UCharacterLevel * ABaseCharacter::GetLevelComponent() const
{
	return LevelComponent;
}

bool ABaseCharacter::IsInteractableInRange(AActor * ToCheck) const
{
	return InteractRangeSphere->IsOverlappingActor(Cast<AActor>(ToCheck)) && Cast<IInteractable>(ToCheck);
}