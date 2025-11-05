// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ConfirmMenuWidgetController.h"

#include "Player/SoulLikePlayerController.h"

void UConfirmMenuWidgetController::SetInputModeWithTag(const FGameplayTag& UIMode)
{
	if(GetSoulLikePC())
	{
		GetSoulLikePC()->SetInputModeTag(UIMode);
	}	
}

void UConfirmMenuWidgetController::BroadcastAddToViewport(FText Message)
{
	OnAddToViewPort.Broadcast(Message);
}

void UConfirmMenuWidgetController::BroadcastRemoveFromParent()
{
	OnRemoveFromParent.Broadcast();
}

void UConfirmMenuWidgetController::BroadcastOnClickedConfirmButton()
{
	OnClickedConfirmButtonDelegate.Broadcast();
}

void UConfirmMenuWidgetController::BroadcastOnClickedCancelButton()
{
	OnClickedCancelButtonDelegate.Broadcast();
}
