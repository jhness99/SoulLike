// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulLikeComboAbility.h"
#include "GA_Rolling.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UGA_Rolling : public USoulLikeComboAbility
{
	GENERATED_BODY()

protected:
	
	virtual void SetupMontage() override;

	virtual void MontageJumpToNextCombo() override;
};
