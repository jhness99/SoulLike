// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Secondary.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UMMC_Secondary : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_Secondary();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat Curve;

private:
	
	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
};
