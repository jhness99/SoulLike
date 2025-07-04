// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ConfirmMenuWidgetController.h"

void UConfirmMenuWidgetController::BroadcastAddToViewport(FText Message)
{
	OnAddToViewPort.Broadcast(Message);
}

void UConfirmMenuWidgetController::BroadcastOnClickedConfirmButton()
{
	OnClickedConfirmButtonDelegate.Broadcast();
}
