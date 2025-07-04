// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SoulLikeAttributeSet.generated.h"

/**
 * Attribute의 Value의 Getter와 Setter, 프로퍼티 자체의 Getter를 생성해주는 매크로
 * @param ClassName 해당 AttributeSet의 이름
 * @param PropertyName 해당 Attribute의 이름
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Effect가 적용될 때 정보를 저장한 FGameplayEffectModCallbackData의 여러 Property를
 * 저장하는 구조체
 */
USTRUCT()
struct FEffectProperties{

	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * TBaseStaticDelegateInstance는 정적 함수를 Delegate화 시켜주는 클래스
 * 클래스를 통해 정적 함수를 Delegate화 하고 그 Delegate의 함수포인터를 가져온다.
 * 해당 함수 포인터를 다시 Delegate로 바꿀 수 있다.
 * ex) DelegateInstance(StaticFuncPtr);
 */
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * AbilitySystem을 가진 객체의 스텟정보를 가지는 오브젝트
 */
UCLASS()
class SOULLIKE_API USoulLikeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	USoulLikeAttributeSet();

	/**
	 * Effect가 해당 AttributeSet에 실행 되었을 때, 그 후 실행되는 Callback함수
	 * 데미지감소, 피격, 경험치 획득 등에 사용할 수 있음
	 * @param Data 실행 된 Effect의 Data를 저장한 구조체
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	/**
	 * Attribute의 값이 바뀌기 전 실행되는 Callback함수
	 * Attribute가 바뀌기 전 이므로 NewValue를 조작해서 바뀌는 값을 제어해줄 수 있다.
	 * @param Attribute Callback의 대상이 되는 Attribute
	 * @param NewValue 바뀔 값, 참조이므로 바뀔값을 임의로 바꿀 수 있다.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 * Attribute의 값이 바뀐 후 실행되는 Callback함수
	 * @param Attribute
	 * @param OldValue
	 * @param NewValue 
	 */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	/**
	 * Attribute를 FGameplayTag와 매칭해서 저장하는 TMap
	 * Attribute를 직접 접근하는 것은 위험하기 때문에 함수를 통해 접근해야한다.
	 * 따라서 특정 함수포인터를 TMap의 Value로써 지정해서 저장한다
	 */
	static TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Vigor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mind, Category = "Primary Attributes")
	FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Mind)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Endurance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Dexterity)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Intelligence)
	
	/**
	 * Secondary Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegeneration, Category = "Vital Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, StaminaRegeneration)

	/**
	 * Resistance Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, LightningResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicResistance, Category = "Resistance Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, MagicResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, PhysicalResistance)

	/**
	 * Vital Attribute
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, Stamina)
	
	/**
	 * Boost Attribute
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthBoost, Category = "Boost Attributes")
	FGameplayAttributeData HealthBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, HealthBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaBoost, Category = "Boost Attributes")
	FGameplayAttributeData StaminaBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, StaminaBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageBoost, Category = "Boost Attributes")
	FGameplayAttributeData DamageBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, DamageBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDamageBoost, Category = "Boost Attributes")
	FGameplayAttributeData PhysicalDamageBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, PhysicalDamageBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireDamageBoost, Category = "Boost Attributes")
	FGameplayAttributeData FireDamageBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, FireDamageBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicDamageBoost, Category = "Boost Attributes")
	FGameplayAttributeData MagicDamageBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, MagicDamageBoost)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningDamageBoost, Category = "Boost Attributes")
	FGameplayAttributeData LightningDamageBoost;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, LightningDamageBoost)

	/**
	 * Meta Attribute
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(USoulLikeAttributeSet, IncomingDamage)

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData& OldMind) const;
	
	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;
    	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
    	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	
	UFUNCTION()
	void OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_HealthBoost(const FGameplayAttributeData& OldHealthBoost) const;

	UFUNCTION()
	void OnRep_StaminaBoost(const FGameplayAttributeData& OldStaminaBoost) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	
	UFUNCTION()
	void OnRep_DamageBoost(const FGameplayAttributeData& OldDamageBoost) const;

	UFUNCTION()
	void OnRep_PhysicalDamageBoost(const FGameplayAttributeData& OldPhysicalDamageBoost) const;

	UFUNCTION()
	void OnRep_FireDamageBoost(const FGameplayAttributeData& OldFireDamageBoost) const;

	UFUNCTION()
	void OnRep_MagicDamageBoost(const FGameplayAttributeData& OldMagicDamageBoost) const;
	
	UFUNCTION()
	void OnRep_LightningDamageBoost(const FGameplayAttributeData& OldLightningDamageBoost) const;

	
private:

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SendHitReactTriggerEvent(AActor* Target, const FGameplayEventData Payload);
	
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);

	void HandleIncomingDamage(const FEffectProperties& Props);

	
	bool bTopOffHealth = true;
	bool bTopOffStamina = true;
};
