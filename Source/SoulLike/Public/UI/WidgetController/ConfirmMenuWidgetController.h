// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "ConfirmMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddToViewportSignature, FText, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveFromParentSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedConfirmMenuButtonSignature);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UConfirmMenuWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:

	void SetInputModeWithTag(const FGameplayTag& UIMode);
	
	UFUNCTION(BlueprintCallable)
	void BroadcastAddToViewport(FText Message);

	UFUNCTION(BlueprintCallable)
	void BroadcastRemoveFromParent();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastOnClickedConfirmButton();

	UFUNCTION(BlueprintCallable)
	void BroadcastOnClickedCancelButton();

	UPROPERTY(BlueprintAssignable)
	FOnAddToViewportSignature OnAddToViewPort;

	UPROPERTY(BlueprintAssignable)
	FOnRemoveFromParentSignature OnRemoveFromParent;

	UPROPERTY(BlueprintAssignable)
	FOnClickedConfirmMenuButtonSignature OnClickedConfirmButtonDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnClickedConfirmMenuButtonSignature OnClickedCancelButtonDelegate;
};
