// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "SavePointMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API USavePointMenuWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:
	
	void SetSavePointName(const FString& InSavePointName) { SavePointName = InSavePointName; }

private:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FString SavePointName;
	
};
