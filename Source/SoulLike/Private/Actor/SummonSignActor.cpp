// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SummonSignActor.h"

#include "SoulLikeFunctionLibrary.h"
#include "SoulLikeGameplayTags.h"
#include "Game/OnlineSessionSubsystem.h"
#include "UI/WidgetController/ConfirmMenuWidgetController.h"


ASummonSignActor::ASummonSignActor()
{
	
}

void ASummonSignActor::Init(const FSummonSignState& InSummonSignState)
{
	SummonSignState = InSummonSignState;
	UE_LOG(LogTemp, Error, TEXT("ASummonSignActor::Init"));
}

void ASummonSignActor::Interaction_Implementation(AActor* InteractionActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Function : %hs"), __FUNCTION__);
	UOnlineSessionSubsystem* OnlineSessionSubsystem = GetGameInstance()->GetSubsystem<UOnlineSessionSubsystem>();
	if(OnlineSessionSubsystem == nullptr) return;

	UConfirmMenuWidgetController* ConfirmMenuWidgetController = USoulLikeFunctionLibrary::GetConfirmMenuWidgetController(this);
	if(ConfirmMenuWidgetController == nullptr) return;

	ConfirmMenuWidgetController->OnClickedConfirmButtonDelegate.Clear();
	ConfirmMenuWidgetController->OnClickedConfirmButtonDelegate.AddDynamic(this, &ASummonSignActor::OnClickedConfirmButton);

	ConfirmMenuWidgetController->OnClickedCancelButtonDelegate.Clear();
	ConfirmMenuWidgetController->OnClickedCancelButtonDelegate.AddDynamic(this, &ASummonSignActor::OnClickedCancelButton);

	FText ConfirmMessage;
	
	if(OnlineSessionSubsystem->CheckIsClientUID(SummonSignState.SteamUID))
	{
		ConfirmMessage = FText::FromString(TEXT("사인을 취소하겠습니까?"));
	}
	else
	{
		ConfirmMessage = FText::FromString(TEXT("소환 하시겠습니까?"));
	}
	
	ConfirmMenuWidgetController->SetInputModeWithTag(FSoulLikeGameplayTags::Get().InputMode_UI);
	ConfirmMenuWidgetController->BroadcastAddToViewport(ConfirmMessage);
}

void ASummonSignActor::OnClickedConfirmButton()
{
	if(UOnlineSessionSubsystem* OnlineSessionSubsystem = GetGameInstance()->GetSubsystem<UOnlineSessionSubsystem>())
	{
		if(!OnlineSessionSubsystem->CheckIsClientUID(SummonSignState.SteamUID) &&
			OnlineSessionSubsystem->SendInviteWithSteamUID(SummonSignState.SteamUID))
		{
			Destroy();
		}
		else OnlineSessionSubsystem->DeleteClientSummonSign();
	}

	UConfirmMenuWidgetController* ConfirmMenuWidgetController = USoulLikeFunctionLibrary::GetConfirmMenuWidgetController(this);
	if(ConfirmMenuWidgetController == nullptr) return;

	ConfirmMenuWidgetController->SetInputModeWithTag(FSoulLikeGameplayTags::Get().InputMode_Game);
}

void ASummonSignActor::OnClickedCancelButton()
{
	UConfirmMenuWidgetController* ConfirmMenuWidgetController = USoulLikeFunctionLibrary::GetConfirmMenuWidgetController(this);
	if(ConfirmMenuWidgetController == nullptr) return;

	ConfirmMenuWidgetController->SetInputModeWithTag(FSoulLikeGameplayTags::Get().InputMode_Game);
	ConfirmMenuWidgetController->BroadcastRemoveFromParent();
}
