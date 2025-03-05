// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_VitalAttributes.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UMMC_VitalAttributes : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UMMC_VitalAttributes();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat Curve;
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayTag AttributeTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	FGameplayTag AttributeBoostTag;

};
