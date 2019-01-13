// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTreeWidget.h"
#include "Characters/BaseCharacter.h"



void USkillTreeWidget::Setup_Implementation(ABaseCharacter * Owner)
{
	this->Owner = Owner;
}

void USkillTreeWidget::CrawlNodes()
{
}



// nodes

void USkillTreeNode::Setup_Implementation(USkillTreeWidget * Owner)
{
	this->Owner = Owner;
}