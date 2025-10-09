// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LobbyHUD.h"

#include "UI/Widgets/SoulLikeUserWidget.h"
#include "UI/WidgetController/ConfirmMenuWidgetController.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	LobbyMenu = CreateWidget<USoulLikeUserWidget>(GetWorld(), LobbyMenuWidgetClass);
	LobbyMenu->AddToViewport();
}

UConfirmMenuWidgetController* ALobbyHUD::GetConfirmMenuWidgetController()
{
	if(ConfirmMenuWidgetController == nullptr){
    
		ConfirmMenuWidgetController = NewObject<UConfirmMenuWidgetController>(this, ConfirmMenuWidgetControllerClass);
	}
	return ConfirmMenuWidgetController;
}