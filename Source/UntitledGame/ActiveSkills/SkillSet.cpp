// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillSet.h"
#include "Items/ItemInfo.h"
#include "Components/Equipment.h"


// Sets default values for this component's properties
USkillSet::USkillSet()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USkillSet::BeginPlay()
{
	Super::BeginPlay();
	// ...

	AddSkills();
}

ESkillSlot USkillSet::MapToSkill(EItemType Item)
{
	switch(Item)
	{
	case EItemType::None:
	case EItemType::Money:
	case EItemType::RareMoney:
	case EItemType::Consumable:
	case EItemType::Other:
		break;
	case EItemType::WeaponModifier:
		return ESkillSlot::Primary;
	case EItemType::ShieldGenerator:
		return ESkillSlot::Secondary;
		break;
	case EItemType::Gloves:
		break;
	case EItemType::Boots:
		break;
	case EItemType::Implant:
		break;
	case EItemType::Relic:
		break;
		break;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("USkillSet::MapToSkill trying to map unknown item"));
		break;
	}
	return ESkillSlot::None;
}

void USkillSet::SetDefaultAttack(TSubclassOf<UBaseSkill> Default)
{
	if(!Default.Get())
		return;

	DefaultBasicAttack = Default;
}

bool USkillSet::ChangeSkill(ESkillSlot Slot, TSubclassOf<UBaseSkill> NewSkill, bool bSendDelegate)
{
	if(!NewSkill.Get())
	{
		ClearSkill(Slot);
		return true;
	}

	UBaseSkill * CreatedSkill = NewObject<UBaseSkill>(this, NewSkill.Get());

	if(!CreatedSkill)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't create skill"));
		return false;
	}

	CreatedSkill->RegisterComponent();

	if(CurrentSkills.Contains(Slot) && IsValid(CurrentSkills[Slot]))
		CurrentSkills[Slot]->DestroyComponent();
	if(!CurrentSkills.Contains(Slot))
		CurrentSkills.Add(Slot, nullptr);

	CurrentSkills[Slot] = CreatedSkill;

	if(bSendDelegate)
		OnChange.Broadcast();
	return true;
}

void USkillSet::ClearSkill(ESkillSlot Slot, bool bSendDelegate)
{
	if(CurrentSkills.Contains(Slot) && IsValid(CurrentSkills[Slot]))
		CurrentSkills[Slot]->DestroyComponent();
	if(!CurrentSkills.Contains(Slot))
		CurrentSkills.Add(Slot, nullptr);
	else
		CurrentSkills[Slot] = nullptr;

	if(Slot == ESkillSlot::Primary && DefaultBasicAttack.Get())
	{
		if(DefaultBasicAttack.Get())
		{
			UBaseSkill * DefaultReplacement = NewObject<UBaseSkill>(this, DefaultBasicAttack.Get());
			DefaultReplacement->RegisterComponent();
			CurrentSkills[Slot] = DefaultReplacement;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s removed basic attack and has no replacement"));
		}
	}

	if(bSendDelegate)
		OnChange.Broadcast();
}

bool USkillSet::UseSkill(ESkillSlot Slot, ABaseEntity * User, ABaseEntity * Target, const FVector & TargetLocation)
{
	return GetSkill(Slot) ? GetSkill(Slot)->Use(User, Target, TargetLocation) : false;
}

bool USkillSet::HasSkill(ESkillSlot Slot) const
{
	return CurrentSkills.Contains(Slot) && IsValid(CurrentSkills[Slot]);
}

UBaseSkill * USkillSet::GetSkill(ESkillSlot Slot) const
{
	return CurrentSkills.Contains(Slot) ? CurrentSkills[Slot] : nullptr;
}

const TMap<ESkillSlot, UBaseSkill*>& USkillSet::GetCurrentSkills()
{
	return CurrentSkills;
}

void USkillSet::AttachEquipment(UEquipment * Eq)
{
	if(!IsValid(Eq))
		return;

	ObservedEq = Eq;
	ObservedEq->OnChange.AddDynamic(this, &USkillSet::UpdateFromEqupiment);
}

void USkillSet::AddSkills()
{
	int8 SkillNumber = 0;
	for(auto SkillTemplate : SkillClasses)
	{
		bool bAdded = ChangeSkill(SkillTemplate.Key, SkillTemplate.Value);
		if(!bAdded)
		{
			UE_LOG(LogTemp, Error, TEXT("Skill number %d on actor %s was added to table, but has no class"), SkillNumber, *GetOwner()->GetName());
		}
		++SkillNumber;
	}

	OnChange.Broadcast();
}

void USkillSet::UpdateFromEqupiment()
{
	if(!ObservedEq)
		return;
	for(EItemType Item : FItemInfo::GetEqItemTypes())
	{
		ESkillSlot CurrentSlot = USkillSet::MapToSkill(Item);
		if(CurrentSlot == ESkillSlot::None)
			continue;
		if(ObservedEq->HasItem(Item))
		{
			ChangeSkill(CurrentSlot, ObservedEq->GetItem(Item).GrantedSkill, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Changing"));
			ClearSkill(CurrentSlot, false);
		}
	}
	OnChange.Broadcast();
}