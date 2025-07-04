// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulLikeGameplayAbility.h"
#include "UsingToolGameplayAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class URegisterableItemInstance;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class SOULLIKE_API UUsingToolGameplayAbility : public USoulLikeGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UFUNCTION()
	void ReceiveEvent(FGameplayEventData Payload);

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

	UPROPERTY()
	TObjectPtr<URegisterableItemInstance> ToolItemInstance = nullptr;
};
