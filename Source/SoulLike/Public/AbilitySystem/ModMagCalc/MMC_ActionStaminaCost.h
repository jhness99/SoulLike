// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_ActionStaminaCost.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UMMC_ActionStaminaCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_ActionStaminaCost();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
