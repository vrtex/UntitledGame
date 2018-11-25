// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(GetCapsuleComponent());

	GetCapsuleComponent()->SetCollisionProfileName(DamagableCollisionPreset);

	NoticeRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Notice Range Sphere"));
	NoticeRangeSphere->SetupAttachment(GetRootComponent());
	NoticeRangeSphere->bHiddenInGame = false;

	ForgetRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Forget Range Sphere"));
	ForgetRangeSphere->SetupAttachment(GetRootComponent());
	ForgetRangeSphere->bHiddenInGame = false;

	DropInventory = CreateDefaultSubobject<UInventory>(FName("Drop inv"));
	DropTable = CreateDefaultSubobject<UDropTable>(FName("Drop table"));

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	NoticeRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	ForgetRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	NoticeRangeSphere->SetHiddenInGame(false);
	ForgetRangeSphere->SetHiddenInGame(false);

	
	GetMesh()->bRenderCustomDepth = true;
	SetAttackRange(AttackRange);
	NoticeRangeSphere->SetSphereRadius(NoticeRange);
	ForgetRangeSphere->SetSphereRadius(ForgetRange);



	SetTeamLabel(GenericEnemyTeam);

	DropTable->FillInventory(DropInventory);
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseEnemy::Interact(AController * InstigatorController, APawn * InstigatorPawn)
{
	return false;
}

void ABaseEnemy::Hightlight(bool bTurnOn)
{
	GetMesh()->SetRenderCustomDepth(bTurnOn);
}

FText ABaseEnemy::GetInteractableName_Implementation() const
{
	return DisplayName;
}

bool ABaseEnemy::ReceiveDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
{
	bool bDealtSomething = Super::ReceiveDamage(Damage, DealtDamage, DamageDealer, Instigator);
	if(IsDead())
	{
		Die();
	}
	return bDealtSomething;
}

float ABaseEnemy::Attack(AActor * Target)
{
	if(!IsValid(Target))
		return 0.0f;
	ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(Target);
	if(!AsBaseEntity)
		return 0.0f;

	GetCharacterMovement()->StopMovementImmediately();
	FaceActor(Target);
	FDamageInfo DamageInfo;
	DamageInfo.PhysicalDamage[EDamageElement::None] = 10;
	DamageInfo.PhysicalDamage[EDamageElement::Fire] = 10;
	FDamageInfo DealtDamageInfo;

	AsBaseEntity->ReceiveDamage(DamageInfo, DealtDamageInfo, this, GetController());

	return 0.0f;
}

void ABaseEnemy::Die()
{
	DropInventory->DropAll();
	Destroy();
}