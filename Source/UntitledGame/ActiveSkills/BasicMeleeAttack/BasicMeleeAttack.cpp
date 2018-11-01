// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicMeleeAttack.h"




void UBasicMeleeAttack::BeginPlay()
{
	Super::BeginPlay();
	RangeSphere = NewObject<UDetectionSphere>(GetOwner(), UDetectionSphere::StaticClass());
	Range = 100.f;
	if(!RangeSphere)
	{
		UE_LOG(LogTemp, Error, TEXT("Lel"));
		return;
	}
	RangeSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	RangeSphere->SetRelativeTransform(FTransform());
	RangeSphere->SetSphereRadius(Range);
	RangeSphere->SetDetectedChannel(ECC_Damagable);
	RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &UBasicMeleeAttack::EnemyDetected);
}

bool UBasicMeleeAttack::Use_Implementation(ABaseEntity * User, ABaseEntity * Target, FVector TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Using"));
	return true;
}

void UBasicMeleeAttack::EnemyDetected(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Something happened"));
}