// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LobbyHUD.h"

#include "UI/Widgets/SoulLikeUserWidget.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	LobbyMenu = CreateWidget<USoulLikeUserWidget>(GetWorld(), LobbyMenuWidgetClass);
	LobbyMenu->AddToViewport();
}
