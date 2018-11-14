// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "BaseEntity.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collider = CreateDefaultSubobject<UDetectionSphere>(FName("Collider"));
	Collider->SetupAttachment(GetRootComponent());
	SetRootComponent(Collider);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	// Movement->bAutoActivate = false;
	Movement->SetVelocityInLocalSpace(FVector(0, 1, 0));
	Movement->InitialSpeed = 500.f;

	InitialLifeSpan = 5.f;

	// Movement->bConstrainToPlane = true;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if(Collider)
	{
		Collider->SetDetectedChannel(ECC_Damagable);
		Collider->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::SomethingDetected);
	}
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::SetOwnerEntity(ABaseEntity * NewOwner)
{
	Owner = NewOwner;
}

void ABaseProjectile::SomethingDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
	if(!Owner)
		return;
	ABaseEntity * AsBaseEntity = Cast<ABaseEntity>(OtherActor);
	if(!AsBaseEntity || Owner->GetRelationTowards(AsBaseEntity->GetTeamLabel()) != ETeamRelation::Hostile)
		return;

	// TODO: poll damage from owner's stats
	FDamageInfo Damage;
	Damage.PhysicalDamage[EDamageElement::None] = 50;
	if(IsValid(Owner))
		Owner->DealDamageTo(AsBaseEntity, Damage, Damage);
	Destroy();
}