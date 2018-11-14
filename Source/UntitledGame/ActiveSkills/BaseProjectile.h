// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Includes.h"
#include "Components/DetectionSphere.h"
#include "BaseProjectile.generated.h"

UCLASS()
class UNTITLEDGAME_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetOwnerEntity(class ABaseEntity * NewOwner);

	
protected:
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent * Movement = nullptr;

	UPROPERTY(VisibleAnywhere)
		UDetectionSphere * Collider = nullptr;


private:
	UFUNCTION()
		void SomethingDetected(UPrimitiveComponent * OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	class ABaseEntity * Owner = nullptr;

};
