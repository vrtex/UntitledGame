// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicRangedAttack.h"
#include "BaseEntity.h"



void UBasicRangedAttack::BeginPlay()
{
	Super::BeginPlay();

	Range = 500.f;

	RangeSphere = NewObject<UDetectionSphere>(GetOwner(), UDetectionSphere::StaticClass());
	if(!RangeSphere)
		return;
	RangeSphere->RegisterComponent();
}

bool UBasicRangedAttack::Use_Implementation(ABaseEntity * User, ABaseEntity * Target, FVector TargetLocation)
{
	if(!ensure(User) || !ensure(ProjectileClass.Get()))
	{
		return false;
	}

	if(User->GetMana() < 20.f)
	{
		User->UseSkill(ESkillSlot::Primary, Target, TargetLocation);
		return false;
	}

	User->RemoveMana(20.f);
	FRotator Rotation;
	TargetLocation = Target ? Target->GetActorLocation() : TargetLocation;
	TargetLocation.Z = User->GetActorLocation().Z;
	Rotation = UKismetMathLibrary::FindLookAtRotation(User->GetActorLocation(), TargetLocation);
	FVector Location = GetOwner()->GetActorLocation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	ABaseProjectile * SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass.Get(), SpawnTransform);
	SpawnedProjectile->SetOwnerEntity(User);
	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
	return true;
}

void UBasicRangedAttack::Cancel_Implementation()
{
}