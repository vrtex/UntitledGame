// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicMeleeAttack.h"
#include "BaseEntity.h"



void UBasicMeleeAttack::BeginPlay()
{
	Super::BeginPlay();
	RangeSphere = NewObject<UDetectionSphere>(GetOwner(), UDetectionSphere::StaticClass());
	Range = 200.f;
	if(!RangeSphere)
	{
		UE_LOG(LogTemp, Error, TEXT("BasicAttack on %s has no range"), *GetOwner()->GetName());
		return;
	}
	RangeSphere->RegisterComponent();
	RangeSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	RangeSphere->SetRelativeTransform(FTransform());
	RangeSphere->SetHiddenInGame(false);
	RangeSphere->SetSphereRadius(Range);
	RangeSphere->SetDetectedChannel(ECC_Damagable);

}

bool UBasicMeleeAttack::Use_Implementation(ABaseEntity * User, ABaseEntity * Target, FVector TargetLocation)
{
	ClearTarget();
	if(!ensure(User))
	{
		UE_LOG(LogTemp, Warning, TEXT("Error during basic melee atack use c++. No User"));
		return false;
	}
	if(Target && User->GetRelationTowards(Target->GetTeamLabel()) != ETeamRelation::Hostile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s trying to attack non-hostile target, something went wrong (basic melee attack)"), *User->GetName());
		return false;
	}
	CurrentUser = User;
	CurrentTarget = Target;
	if(RangeSphere->IsOverlappingActor(CurrentTarget))
	{
		EnemyDetected(nullptr, CurrentTarget, nullptr, 0, false, FHitResult());
		return true;
	}
	if(Target)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(CurrentUser->GetController(), CurrentTarget);
		RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &UBasicMeleeAttack::EnemyDetected);
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(CurrentUser->GetController(), TargetLocation);
	}
	return true;
}

void UBasicMeleeAttack::Cancel_Implementation()
{
	if(CurrentUser)
		CurrentUser->GetCharacterMovement()->StopMovementImmediately();
	ClearTarget();
}

void UBasicMeleeAttack::BeginDestroy()
{
	Super::BeginDestroy();
	if(!RangeSphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something fucky"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Something not fucky"));
	RangeSphere->DestroyComponent();
}

void UBasicMeleeAttack::EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor != CurrentTarget)
		return;
	Attack();
}

float UBasicMeleeAttack::Attack()
{
	FDamageInfo Damage;
	Damage.PhysicalDamage[EDamageElement::None] = 10.f;
	CurrentUser->DealDamageTo(CurrentTarget, Damage, Damage);
	CurrentUser->FaceActor(CurrentTarget);
	Cancel();
	return Damage.GetTotalDamage();
}

void UBasicMeleeAttack::ClearTarget()
{
	CurrentTarget = nullptr;
	CurrentUser = nullptr;
	RangeSphere->OnComponentBeginOverlap.RemoveDynamic(this, &UBasicMeleeAttack::EnemyDetected);
}
