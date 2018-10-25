// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "DetectionSphere.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLEDGAME_API UDetectionSphere : public USphereComponent
{
	GENERATED_BODY()
	

public:

	void SetDetectedChannel(ECollisionChannel Channel);
	
	void SetDetectedChannel(TArray<ECollisionChannel> Channel);
	
};
