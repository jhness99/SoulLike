
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton Object
 * Native로 선언한 GameplayTag를 C++에서 접근하기 쉽게 하기 위해 사용
 */
struct FSoulLikeGameplayTags
{
public:

	static const FSoulLikeGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/**
	 * Primary Attribute
	 * 기본 Attribute
	 */
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Mind;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	
	/**
	 * Secondary Attribute
	 * Primary Attribute에 의해 값이 바뀌는 Attribute
	 */
	FGameplayTag Attributes_Secondary_Health;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_Stamina;
	FGameplayTag Attributes_Secondary_MaxStamina;

	/**
	 * Boost Attribute
	 */
	FGameplayTag Attributes_Boost_HealthBoost;
	FGameplayTag Attributes_Boost_StaminaBoost;

	/**
	 * Input Tag
	 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;

	/**
	 * Event Tag
	 */
	FGameplayTag Event_Montage_WaitInput;
	FGameplayTag Event_Montage_NextAction;

	
	bool bInit = false;
	
private:
	
	static FSoulLikeGameplayTags GameplayTags;

	
};