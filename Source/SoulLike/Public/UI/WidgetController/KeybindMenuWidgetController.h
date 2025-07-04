// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "GameplayTagContainer.h"
#include "KeybindMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveInputTagSignature, const FGameplayTag&, AbilityTag);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UKeybindMenuWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnReceiveInputTagSignature OnReceiveInputTagDelegate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag SelectedAbilityTag;
};
