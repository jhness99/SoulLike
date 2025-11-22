// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SL_SubsystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config=SubsystemSettings, DefaultConfig)
class SOULLIKE_API USL_SubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	/** OnlineSessionSubsystem Properties*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "OnlineSessionSubsystem")
	TSubclassOf<AActor> WhiteSignActorClass;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "OnlineSessionSubsystem")
	float MaxSearchSignDistance = 750.f;
};
