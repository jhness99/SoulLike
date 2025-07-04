// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SoulLikeWidgetController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "Kismet/GameplayStatics.h"

#include "Player/SoulLikePlayerController.h"
#include "Player/SoulLikePlayerState.h"


void USoulLikeWidgetController::BroadcastInitialValues()
{
	
}

void USoulLikeWidgetController::BindCallbacksToModels()
{
	
}

void USoulLikeWidgetController::BindCallbacksToUISubsystem()
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
	{
		if(UUISubSystem* UISubSystem = GI->GetSubsystem<UUISubSystem>())
		{
			UISubSystem->OnReceiveUITask.AddDynamic(this, &USoulLikeWidgetController::BroadcastReceiveUITask);
		}
	}
}

void USoulLikeWidgetController::BroadcastCloseMenu()
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
	{
		if(UUISubSystem* UISubSystem = GI->GetSubsystem<UUISubSystem>())
		{
			UISubSystem->OnCloseMainMenu.Execute();
		}
	}
}

void USoulLikeWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

ASoulLikePlayerController* USoulLikeWidgetController::GetSoulLikePC()
{
	if(SoulLikePlayerController == nullptr)
	{
		SoulLikePlayerController = Cast<ASoulLikePlayerController>(PlayerController);
	}
	return SoulLikePlayerController;
}

ASoulLikePlayerState* USoulLikeWidgetController::GetSoulLikePS()
{
	if(SoulLikePlayerState == nullptr)
	{
		SoulLikePlayerState = Cast<ASoulLikePlayerState>(PlayerState);
	}
	return SoulLikePlayerState;
}

USoulLikeAbilitySystemComponent* USoulLikeWidgetController::GetSoulLikeASC()
{
	if(SoulLikeAbilitySystemComponent == nullptr)
	{
		SoulLikeAbilitySystemComponent = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent);
	}
	return SoulLikeAbilitySystemComponent;
}

USoulLikeAttributeSet* USoulLikeWidgetController::GetSoulLikeAS()
{
	if(SoulLikeAttributeSet == nullptr)
	{
		SoulLikeAttributeSet = Cast<USoulLikeAttributeSet>(AttributeSet);
	}
	return SoulLikeAttributeSet;
}

void USoulLikeWidgetController::BroadcastReceiveUITask(const FGameplayTag& TaskTag)
{
	OnReceiveUITask.Broadcast(TaskTag);
}
