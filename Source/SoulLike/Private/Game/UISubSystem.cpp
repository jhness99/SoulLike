
// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UISubSystem.h"


void UUISubSystem::ReceiveUITask(const FGameplayTag& TaskTag) const
{
	OnReceiveUITask.Broadcast(TaskTag);
}

void UUISubSystem::CloseMenu()
{
	if(OnCloseMainMenu.IsBound())
	{
		OnCloseMainMenu.Execute();
	}
}
