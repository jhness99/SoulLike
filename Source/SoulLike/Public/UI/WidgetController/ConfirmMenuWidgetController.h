// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "ConfirmMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddToViewportSignature, FText, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedConfirmButtonSignature);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UConfirmMenuWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void BroadcastAddToViewport(FText Message);
	
	UFUNCTION(BlueprintCallable)
	void BroadcastOnClickedConfirmButton();

	UPROPERTY(BlueprintAssignable)
	FOnAddToViewportSignature OnAddToViewPort;

	UPROPERTY(BlueprintAssignable)
	FOnClickedConfirmButtonSignature OnClickedConfirmButtonDelegate;
};
