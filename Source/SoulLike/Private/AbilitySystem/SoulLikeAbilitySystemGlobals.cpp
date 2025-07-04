// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAbilitySystemGlobals.h"

#include "SoulLikeAbilityTypes.h"

FGameplayEffectContext* USoulLikeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSoulLikeGameplayEffectContext();
}
