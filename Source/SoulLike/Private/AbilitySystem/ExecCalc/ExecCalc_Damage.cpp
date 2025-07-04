// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "Inventory/EquipmentItemInstance.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"
#include "SoulLikeAbilityTypes.h"


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().PhysicalResistanceDef);

	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().StrengthDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().DexterityDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().IntelligenceDef);

	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().DamageBoostDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().PhysicalDamageBoostDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().FireDamageBoostDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().MagicDamageBoostDef);
	RelevantAttributesToCapture.Add(FSoulLikeExecCalcCaptureStatics::CaptureStatics().LightningDamageBoostDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	int32 WeaponUpgradeLevel = 0;
	if(SourceAvatar && SourceAvatar->Implements<UCombatInterface>())
	{
		if(UEquipmentItemInstance* WeaponInstance = ICombatInterface::Execute_GetCurrentWeapon(SourceAvatar))
		{
			WeaponUpgradeLevel = WeaponInstance->GetUpgradeLevel();
		}
	}

	float Damage = 0;
	for(const TPair<FGameplayTag, FGameplayTag>& Pair : FSoulLikeGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		const FGameplayTag AttributeTag = FSoulLikeGameplayTags::Get().DamageTypesToBoostAttribute[DamageTypeTag];
		const FGameplayTag DamageBoostTag = FSoulLikeGameplayTags::Get().DamageTypesToDamageBoost[DamageTypeTag];

		checkf(FSoulLikeExecCalcCaptureStatics::CaptureStatics().TagsToCaptureResistanceDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag : [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureResistanceDef = FSoulLikeExecCalcCaptureStatics::CaptureStatics().TagsToCaptureResistanceDefs[ResistanceTag];
		const FGameplayEffectAttributeCaptureDefinition CaptureAttributeTagDef = FSoulLikeExecCalcCaptureStatics::CaptureStatics().TagsToCaptureAttributesDefs[AttributeTag];
		const FGameplayEffectAttributeCaptureDefinition CaptureDamageBoostTagDef = FSoulLikeExecCalcCaptureStatics::CaptureStatics().TagsToCaptureDamageBoostDefs[DamageBoostTag];
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
		if(DamageTypeValue <= 0.f)
		{
			continue;
		}

		DamageTypeValue *= ( 1 + ( 0.2 * WeaponUpgradeLevel));

		float TotalDamageBoostValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FSoulLikeExecCalcCaptureStatics::CaptureStatics().DamageBoostDef, EvaluateParameters, TotalDamageBoostValue);
		DamageTypeValue *= ((TotalDamageBoostValue) + 1);
		
		float DamageBoostValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDamageBoostTagDef, EvaluateParameters, DamageBoostValue);
		DamageTypeValue *= ((DamageBoostValue) + 1);
		
		float AttributeValue = 1.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributeTagDef, EvaluateParameters, AttributeValue);
		DamageTypeValue *= ((AttributeValue * 0.02) + 1);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureResistanceDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) * 0.01f;
		
		Damage += DamageTypeValue;
	}
	
	const FGameplayModifierEvaluatedData EvalutedData(USoulLikeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvalutedData);
}
