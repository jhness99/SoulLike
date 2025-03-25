// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_ActionStaminaCost.h"
#include "AbilitySystem/Data/AbilityInfo.h"

#include "Interface/CombatInterface.h"

#include "Abilities/GameplayAbility.h"

#include "SoulLikeItemTypes.h"
#include "SoulLikeGameplayTags.h"
#include "SoulLikeFunctionLibrary.h"

UMMC_ActionStaminaCost::UMMC_ActionStaminaCost()
{
	
}

float UMMC_ActionStaminaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FSoulLikeGameplayTags GameplayTags = FSoulLikeGameplayTags::Get();
	
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	const UGameplayAbility* ActivateAbility = ContextHandle.GetAbilityInstance_NotReplicated();
	
	FGameplayTag AbilityTag = ActivateAbility->AbilityTags.First();
	if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Attack))
	{
		if(Spec.GetContext().GetSourceObject() && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
		{
			if(UWeaponData* WeaponData = Cast<UWeaponData>(ICombatInterface::Execute_GetCurrentWeaponItemData(Spec.GetContext().GetSourceObject())))
			{
				return -WeaponData->Stamina;
			}
		}
	}
	else
	{
		if(Spec.GetContext().GetSourceObject())
		{
			FSoulLikeAbilityInfo AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfoForTag(Spec.GetContext().GetSourceObject(), AbilityTag);
			return -AbilityInfo.StaminaCost;
		}
	}
	
	return 0.f;
}
