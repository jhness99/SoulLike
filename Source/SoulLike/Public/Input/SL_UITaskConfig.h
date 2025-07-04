// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SL_UITaskConfig.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USL_UITaskConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FGameplayTag> InputTagToUITask;
};
