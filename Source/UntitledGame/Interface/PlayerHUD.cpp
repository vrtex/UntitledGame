// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUD.h"




void UPlayerHUD::Setup_Implementation(ABaseCharacter * Character)
{
	if(!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't setup Player hud"));
		return;
	}

	PlayerChar = Character;
}