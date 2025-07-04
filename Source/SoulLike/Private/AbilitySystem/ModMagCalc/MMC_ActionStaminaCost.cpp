// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_ActionStaminaCost.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

#include "Interface/CombatInterface.h"

#include "Abilities/GameplayAbility.h"

#include "SoulLikeItemTypes.h"
#include "SoulLikeGameplayTags.h"
#include "SoulLikeFunctionLibrary.h"

UMMC_ActionStaminaCost::UMMC_ActionStaminaCost()
{
	StaminaDef.AttributeToCapture = USoulLikeAttributeSet::GetStaminaAttribute();
	StaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	StaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StaminaDef);
}

float UMMC_ActionStaminaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FSoulLikeGameplayTags GameplayTags = FSoulLikeGameplayTags::Get();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float StaminaCost = 0.0f;
	
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	const UGameplayAbility* ActivateAbility = ContextHandle.GetAbilityInstance_NotReplicated();
	
	FGameplayTag AbilityTag = ActivateAbility->AbilityTags.First();
	if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Attack))
	{
		if(Spec.GetContext().GetSourceObject() && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
		{
			if(UWeaponData* WeaponData = Cast<UWeaponData>(ICombatInterface::Execute_GetCurrentWeaponItemData(Spec.GetContext().GetSourceObject())))
			{
				StaminaCost = -WeaponData->Stamina;
			}
		}
	}
	else
	{
		if(Spec.GetContext().GetSourceObject())
		{
			FSoulLikeAbilityInfo AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfoForTag(Spec.GetContext().GetSourceObject(), AbilityTag);
			StaminaCost = -AbilityInfo.StaminaCost;
		}
	}
	
	float Stamina = 0.f;
	GetCapturedAttributeMagnitude(StaminaDef, Spec, EvaluationParameters, Stamina);

	Stamina = FMath::Max<float>(Stamina, 0.f);

	if(Stamina > 0 && FMath::Abs(StaminaCost) >= Stamina)
	{
		StaminaCost = -Stamina;
	}
	
	return StaminaCost;
}
