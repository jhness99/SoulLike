// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SoulLikeAbilityTypes.h"
#include "SoulLikeAbilitySystemComponent.generated.h"

class UKeybindMenuWidgetController;
class USoulLikeSaveGame;

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputTagStartedWaitingSignature, const FGameplayTag& /*InputTag*/);

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	
	void GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, APawn* Instigator);
	void GiveAbilitiesFromSaveData(USoulLikeSaveGame* SaveData, APawn* Instigator);
	
	void ChangeAbilityInputTag(UKeybindMenuWidgetController* KeybindMenuWidgetController, const FGameplayTag& InputTag);

	//void UpgradeAttributes(TMap<FGameplayTag, float> ValueToAttributes);
	void UpgradeAttributes(const TArray<FAttributeValue>& AttributeValues);
	
	UFUNCTION(Server, Reliable)
	//void Server_UpgradeAttributes(UGameplayEffect* AttributeEffect);
	void Server_UpgradeAttributes(const TArray<FAttributeValue>& AttributeValues);

	UFUNCTION(Server, Reliable)
	void Server_ApplyDamageEffect(FDamageEffectParams DamageParams);
	
	void TryActivateAbilityFromAbilityTag(const FGameplayTag& AbilityTag);
	void TryActivateAbilityFromInputTag(const FGameplayTag& InputTag);
	/**
	 * InputTag를 가지고 있는 Ability를 활성화 시키는 함수
	 * @param InputTag 
	 */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
	FOnInputTagStartedWaitingSignature WaitInputDelegate;

protected:

	FGameplayAbilitySpec* GetAbilitySpecFromInputTag(const FGameplayTag& InputTag);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecFromInputTag(const FGameplayTag& InputTag);
	
};
