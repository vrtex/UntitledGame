// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Includes.h"
#include "Interfaces/DamageInfo.h"
#include "TeamInterface.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeamRelation : uint8
{
	None,
	Friendly,
	Hostile,
	Neutral
};

UINTERFACE(BlueprintType)
class UNTITLEDGAME_API UTeamInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class UNTITLEDGAME_API ITeamInterface
{
	GENERATED_IINTERFACE_BODY()
public:

	virtual void SetTeamLabel(uint8 OtherLabel) = 0;

	virtual uint8 GetTeamLabel() const = 0;

	virtual ETeamRelation GetRelationTowards(uint8 OtherLabel) const
	{
		if(GetTeamLabel() == GenericNeutralTeam || OtherLabel == GenericNeutralTeam)
			return ETeamRelation::Neutral;
		if(GetTeamLabel() == OtherLabel)
			return ETeamRelation::Friendly;
		return ETeamRelation::Hostile;
	}

	virtual bool DealDamage(const FDamageInfo & Damage, FDamageInfo & DealtDamage, class ABaseEntity * DamageDealer, AController * Instigator) = 0;

	// TODO: make this blueprint native event
	// UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	virtual float GetResistTo(EDamageElement Element, EDamageType Type) const { return 0.f; };

};