// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulLikeComboAbility.h"
#include "RollingAbility.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API URollingAbility : public USoulLikeComboAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void SetupMontage() override;
	virtual void MontageJumpToNextCombo() override;
	
	virtual FName GetSectionName() override;

	UPROPERTY()
	FRotator WarpingRotation;

private:

	UFUNCTION()
	void OnReceiveEvent(FGameplayEventData Payload);

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEffectEventTask;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> ApplyEffectClass;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveGEHandle;
};
