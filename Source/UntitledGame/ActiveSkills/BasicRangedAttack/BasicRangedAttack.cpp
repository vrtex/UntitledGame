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
	CurrentUser = User;
	CurrentTarget = Target;
	CurrentTargetLocation = TargetLocation;
	if(!ensure(CurrentUser) || !ensure(ProjectileClass.Get()))
	{
		return false;
	}

	if(!CurrentUser->CanReact())
		return false;

	if(User->GetMana() < 20.f)
	{
		User->UseSkill(ESkillSlot::Primary, Target, TargetLocation);
		return false;
	}

	CurrentUser->StopMovement();

	if(CurrentTarget)
		CurrentUser->FaceActor(CurrentTarget);
	else
		CurrentUser->FaceLocation(CurrentTargetLocation);
	CurrentUser->SetCurrentState(EEntityState::RangedAttack);

	return true;
}

void UBasicRangedAttack::Cancel_Implementation()
{
	CurrentTarget = nullptr;
	if(!CurrentUser)
		return;

}

void UBasicRangedAttack::Finish_Implementation()
{
	if(!CurrentUser)
		return;


	CurrentUser->RemoveMana(20.f);
	
	
	FRotator Rotation;
	CurrentTargetLocation = CurrentTarget ? CurrentTarget->GetActorLocation() : CurrentTargetLocation;
	CurrentTargetLocation.Z = CurrentUser->GetActorLocation().Z;
	Rotation = UKismetMathLibrary::FindLookAtRotation(CurrentUser->GetActorLocation(), CurrentTargetLocation);
	FVector SpawnLocation = GetOwner()->GetActorLocation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	ABaseProjectile * SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass.Get(), SpawnTransform);
	SpawnedProjectile->SetOwnerEntity(CurrentUser);
	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);

	Cancel();
}