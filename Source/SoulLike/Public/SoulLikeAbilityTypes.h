#pragma once

#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "SoulLikeItemTypes.h"
#include "SoulLikeGameplayTags.h"
#include "SoulLikeAbilityTypes.generated.h"

class UGameplayEffect;
struct FWeaponDamageInfo;

UENUM(Blueprintable, BlueprintType)
enum class EMeleeTraceType : uint8
{
	None	UMETA(DisplayName = "None"),
	Right  UMETA(DisplayName = "Right"),
	Left	UMETA(DisplayName = "Left"),
};

struct FSoulLikeExecCalcCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);

	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamageBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireDamageBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDamageBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningDamageBoost);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaBoost);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureResistanceDefs;
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureAttributesDefs;
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDamageBoostDefs;
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureVitalBoostDefs;

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturePassiveDefs;
	
	bool bInit = false;

	FSoulLikeExecCalcCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, PhysicalResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, Dexterity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, Intelligence, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, DamageBoost, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, PhysicalDamageBoost, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, FireDamageBoost, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, MagicDamageBoost, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, LightningDamageBoost, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, HealthBoost, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulLikeAttributeSet, StaminaBoost, Source, false);
	}

	void Init(){

		const FSoulLikeGameplayTags& Tags = FSoulLikeGameplayTags::Get();
		if(!Tags.bInit || bInit) return;

		bInit = true;

		TagsToCaptureResistanceDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureResistanceDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureResistanceDefs.Add(Tags.Attributes_Resistance_Magic, MagicResistanceDef);
		TagsToCaptureResistanceDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);

		TagsToCaptureAttributesDefs.Add(Tags.Attributes_Primary_Strength, StrengthDef);
		TagsToCaptureAttributesDefs.Add(Tags.Attributes_Primary_Dexterity, DexterityDef);
		TagsToCaptureAttributesDefs.Add(Tags.Attributes_Primary_Intelligence, IntelligenceDef);

		TagsToCaptureDamageBoostDefs.Add(Tags.Attributes_Boost_DamageBoost, DamageBoostDef);
		TagsToCaptureDamageBoostDefs.Add(Tags.Attributes_Boost_PhysicalDamageBoost, PhysicalDamageBoostDef);
		TagsToCaptureDamageBoostDefs.Add(Tags.Attributes_Boost_FireDamageBoost, FireDamageBoostDef);
		TagsToCaptureDamageBoostDefs.Add(Tags.Attributes_Boost_MagicDamageBoost, MagicDamageBoostDef);
		TagsToCaptureDamageBoostDefs.Add(Tags.Attributes_Boost_LightningDamageBoost, LightningDamageBoostDef);

		TagsToCaptureVitalBoostDefs.Add(Tags.Attributes_Boost_HealthBoost, HealthBoostDef);
		TagsToCaptureVitalBoostDefs.Add(Tags.Attributes_Boost_StaminaBoost, StaminaBoostDef);
		
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Primary_Strength, StrengthDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Primary_Dexterity, DexterityDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Primary_Intelligence, IntelligenceDef);

		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_DamageBoost, DamageBoostDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_PhysicalDamageBoost, PhysicalDamageBoostDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_FireDamageBoost, FireDamageBoostDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_MagicDamageBoost, MagicDamageBoostDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_LightningDamageBoost, LightningDamageBoostDef);

		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_HealthBoost, HealthBoostDef);
		TagsToCapturePassiveDefs.Add(Tags.Attributes_Boost_StaminaBoost, StaminaBoostDef);
	}
	
	static const FSoulLikeExecCalcCaptureStatics& CaptureStatics();
};


USTRUCT(BlueprintType, Blueprintable)
struct FAttributeValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float AttributeValue = 0.f;
};


USTRUCT(BlueprintType)
struct FDamageEffectParams{

	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent; 

	UPROPERTY(BlueprintReadWrite)
	TArray<FWeaponDamageInfo> DamageInfos;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag AttackType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float AttackForce = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float Toughness = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;
};

UCLASS(BlueprintType)
class UDamageEffectParamsObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FDamageEffectParams Params;
};

USTRUCT(BlueprintType)
struct FSoulLikeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	float GetAttackForce() const {return AttackForce; }
	float GetToughness() const {return Toughness; }
	FVector GetKnockbackForce() const {return KnockbackForce; }
	
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetAttackForce(float InAttackForce) { AttackForce = InAttackForce; }
	void SetToughness(float InToughness) { Toughness = InToughness; }
	void SetKnockbackForce(const FVector& InKnockbackForce) {KnockbackForce =  InKnockbackForce;}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FSoulLikeGameplayEffectContext* Duplicate() const override
	{
		FSoulLikeGameplayEffectContext* NewContext = new FSoulLikeGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	float AttackForce = 0.f;

	UPROPERTY()
	float Toughness = 0.f;
	
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
	
};

template<>
struct TStructOpsTypeTraits<FSoulLikeGameplayEffectContext> : TStructOpsTypeTraitsBase2<FSoulLikeGameplayEffectContext>{

	enum{

		WithNetSerializer = true,
		WithCopy = true
	};
};
