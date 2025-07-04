// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeSetWidgetController.h"

#include "SoulLikeGameplayTags.h"

#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/SoulLikePlayerState.h"


void UAttributeSetWidgetController::BroadcastInitialValues()
{
	for(auto& Pair : GetSoulLikeAS()->TagsToAttributes){

		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	LevelChangeDelegate.Broadcast(GetSoulLikePS()->GetPlayerLevel());
	ExpChangeDelegate.Broadcast(GetSoulLikePS()->GetExp());
}

void UAttributeSetWidgetController::BindCallbacksToModels()
{
	for(auto& Pair : GetSoulLikeAS()->TagsToAttributes)
	{
		if(Pair.Key.MatchesTagExact(FSoulLikeGameplayTags::Get().Attributes_Vital_Stamina)) continue;
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData Data){

				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}   
		);
	}
	GetSoulLikePS()->OnLevelChangedDelegate.AddLambda([this](int32 Level)
	{
		LevelChangeDelegate.Broadcast(Level);
	});
	GetSoulLikePS()->OnExpChangedDelegate.AddLambda([this](int32 Exp)
	{
		ExpChangeDelegate.Broadcast(Exp);
	});
}

void UAttributeSetWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	check(AttributeInfo);

	FSoulLikeAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
