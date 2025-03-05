// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_VitalAttributes.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "GameplayEffectExecutionCalculation.h"

#include "SoulLikeGameplayTags.h"


struct SL_VitalAttributeStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vigor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaBoost);
 	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	bool bInit = false;
 	
	SL_VitalAttributeStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, Vigor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, Endurance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, HealthBoost, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, StaminaBoost, Target, false);
	}

	void Init(){

		const FSoulLikeGameplayTags& Tags = FSoulLikeGameplayTags::Get();
		if(!Tags.bInit || bInit) return;

		bInit = true;

		TagsToCaptureDefs.Add(Tags.Attributes_Primary_Vigor, VigorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Primary_Endurance, EnduranceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Boost_HealthBoost, HealthBoostDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Boost_StaminaBoost, StaminaBoostDef);
	}
};

static const SL_VitalAttributeStatics& AttributesStatics()
{
	static SL_VitalAttributeStatics AttributesStatics;
	AttributesStatics.Init();
	return AttributesStatics;
}

UMMC_VitalAttributes::UMMC_VitalAttributes()
{
	RelevantAttributesToCapture.Add(AttributesStatics().VigorDef);
	RelevantAttributesToCapture.Add(AttributesStatics().EnduranceDef);
	RelevantAttributesToCapture.Add(AttributesStatics().HealthBoostDef);
	RelevantAttributesToCapture.Add(AttributesStatics().StaminaBoostDef);
}

float UMMC_VitalAttributes::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Value = 0.f;
	GetCapturedAttributeMagnitude(AttributesStatics().TagsToCaptureDefs.FindChecked(AttributeTag), Spec, EvaluationParameters, Value);
	Value = FMath::Max<float>(Value, 0.0f);
	
	Value = Curve.GetValueAtLevel(Value);
	
	float Boost = 0.f;
	GetCapturedAttributeMagnitude(AttributesStatics().TagsToCaptureDefs.FindChecked(AttributeBoostTag), Spec, EvaluationParameters, Boost);
	Boost = FMath::Max<float>(Boost, 0.0f);
	
	Value *= (Boost + 100) * 0.01;
	return Value;
}
