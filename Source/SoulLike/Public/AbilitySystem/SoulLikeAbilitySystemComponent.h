// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SoulLikeAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputTagStartedWaiting, const FGameplayTag& /*InputTag*/);

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/**
	 * InputTag를 가지고 있는 Ability를 활성화 시키는 함수
	 * @param InputTag 
	 */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	FGameplayAbilitySpec GetAbilitySpecFromInputTag(const FGameplayTag& InputTag);

	FOnInputTagStartedWaiting WaitInputDelegate;
	
};
