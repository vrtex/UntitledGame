// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(GetCapsuleComponent());

	GetCapsuleComponent()->SetCollisionProfileName(DamagableCollisionPreset);

	AttackRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Attack Range Sphere"));
	AttackRangeSphere->SetupAttachment(GetRootComponent());

	NoticeRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Notice Range Sphere"));
	NoticeRangeSphere->SetupAttachment(GetRootComponent());

	ForgetRangeSphere = CreateDefaultSubobject<UDetectionSphere>(FName("Forget Range Sphere"));
	ForgetRangeSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->bRenderCustomDepth = true;

	AttackRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	NoticeRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	ForgetRangeSphere->SetDetectedChannel(ECollisionChannel::ECC_Damagable);
	AttackRangeSphere->SetSphereRadius(AttackRange);
	NoticeRangeSphere->SetSphereRadius(NoticeRange);
	ForgetRangeSphere->SetSphereRadius(ForgetRange);

	SetTeamLabel(2);
	//GetCapsuleComponent()->SetCollisionProfileName(EnemyCollisionPreset);
	/*
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Enemy);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_CursorTrace, ECollisionResponse::ECR_Block);
	*/
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

bool ABaseEnemy::DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, ABaseEntity * DamageDealer, AController * Instigator)
{
	bool bDealtSomething = Super::DealDamage(Damage, DealtDamage, DamageDealer, Instigator);
	if(IsDead())
		Destroy();
	return bDealtSomething;
}

void ABaseEnemy::FaceActor(AActor * Target)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		Target->GetActorLocation()
	);
	TargetRotation.Roll = 0;
	TargetRotation.Pitch = 0;
	SetActorRotation(TargetRotation);
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

	AsBaseEntity->DealDamage(DamageInfo, DealtDamageInfo, this, GetController());

	return 0.0f;
}

TArray<AActor*> ABaseEnemy::GetEnemiesInRange() const
{
	TArray<AActor *> Enemies;
	NoticeRangeSphere->GetOverlappingActors(Enemies);
	for(int32 i = Enemies.Num() - 1; i >= 0; --i)
	{
		AActor * Current = Enemies[i];
		ABaseEntity * AsTeamAgent = Cast<ABaseEntity>(Current);
		if((!AsTeamAgent) || 
			GetRelationTowards(AsTeamAgent->GetTeamLabel()) != ETeamRelation::Hostile)
		{
			Enemies.RemoveAt(i);
		}
	}
	return Enemies;
}