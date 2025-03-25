// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetSoulLikeAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetSoulLikeAS()->GetMaxHealth());
	OnStaminaChanged.Broadcast(GetSoulLikeAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetSoulLikeAS()->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToModels()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetSoulLikeAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void const
		{
				OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetSoulLikeAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void const
		{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetSoulLikeAS()->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void const
		{
				OnStaminaChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetSoulLikeAS()->GetMaxStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void const
		{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
		});
}
