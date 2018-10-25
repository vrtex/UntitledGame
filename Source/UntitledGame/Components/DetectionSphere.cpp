// Fill out your copyright notice in the Description page of Project Settings.

#include "DetectionSphere.h"


void UDetectionSphere::SetDetectedChannel(ECollisionChannel Channel)
{
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Overlap);
}

void UDetectionSphere::SetDetectedChannel(TArray<ECollisionChannel> Channel)
{
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	for(ECollisionChannel C : Channel)
	{
		SetCollisionResponseToChannel(C, ECollisionResponse::ECR_Overlap);
	}
}

