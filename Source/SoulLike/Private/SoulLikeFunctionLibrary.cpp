// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeFunctionLibrary.h"

#include "AbilitySystem/Data/AbilityInfo.h"

#include "Game/SoulLikeGameInstance.h"

#include "Player/SoulLikePlayerState.h"

#include "UI/HUD/SoulLikeHUD.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"


bool USoulLikeFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                          ASoulLikeHUD*& OutSoulLikeHUD, FWidgetControllerParams& OutWCParams)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutSoulLikeHUD = Cast<ASoulLikeHUD>(PC->GetHUD());
		if(OutSoulLikeHUD != nullptr)
		{
			ASoulLikePlayerState* PS = PC->GetPlayerState<ASoulLikePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* USoulLikeFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetOverlayWidgetController(Params);
	}
	return nullptr;
}

UAbilityInfo* USoulLikeFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if(USoulLikeGameInstance* GameInstance = Cast<USoulLikeGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return GameInstance->AbilityInfoDataAsset;
	}
	return nullptr;
}

FSoulLikeAbilityInfo USoulLikeFunctionLibrary::GetAbilityInfoForTag(const UObject* WorldContextObject,
	const FGameplayTag& Tag)
{
	if(UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject))
	{
		return AbilityInfo->FindAbilityInfoForTag(Tag);
	}
	return FSoulLikeAbilityInfo();
}
