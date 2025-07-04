// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/LevelUpMenuWidgetController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/SoulLikePlayerState.h"


void ULevelUpMenuWidgetController::UpgradeAttributes(const TArray<FAttributeValue>& AttributeValues)
{
	if(GetSoulLikeASC())
	{
		GetSoulLikeASC()->UpgradeAttributes(AttributeValues);
	}
}

int32 ULevelUpMenuWidgetController::GetTotalExp(int32 Level) const
{
	return AttributeInfo->GetRequireExp(Level);
}

void ULevelUpMenuWidgetController::AddToPlayerLevel(int32 InPlayerLevel)
{
	if(GetSoulLikePS())
	{
		GetSoulLikePS()->AddToPlayerLevel(InPlayerLevel);
	}
}

void ULevelUpMenuWidgetController::AddToExp(int32 InExp)
{
	if(GetSoulLikePS())
	{
		GetSoulLikePS()->AddToExp(InExp);
	}
}

void ULevelUpMenuWidgetController::SetbCanLevelUp(bool InbCanLevelUp)
{
	bCanLevelUp = InbCanLevelUp;
	ChangeCanLevelUpDelegate.Broadcast(bCanLevelUp);
}
