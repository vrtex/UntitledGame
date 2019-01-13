// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentWidget.h"
#include "Characters/BaseCharacter.h"



void UEquipmentWidget::Setup_Implementation(ABaseCharacter * Character)
{
	if(!Character || !Character->Equipment)
	{
		UE_LOG(LogTemp, Error, TEXT("Error during eq widget setup"));
		return;
	}


	Owner = Character;

	Character->Equipment->OnChange.AddDynamic(this, &UEquipmentWidget::UpdateEquipment);
}