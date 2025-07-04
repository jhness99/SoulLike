// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "SoulLikeAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
