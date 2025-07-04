// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulLikeAbilityTypes.h"
#include "AbilitySystem/Abilities/SoulLikeDamageGameplayAbility.h"
#include "GA_Riposte.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class SOULLIKE_API UGA_Riposte : public USoulLikeDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Riposte();

protected:
	
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:

	UFUNCTION()
	void ReceiveMontageEvent(FGameplayEventData Payload);
	
	void SetupMontage(const FGameplayEventData* TriggerEventData, UAnimMontage*& Montage, FName& Section) const;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;

	UPROPERTY()
	TObjectPtr< AActor> VictimActor;

	UPROPERTY()
	FGameplayTagContainer DeathStatusTags;

	UPROPERTY()
	bool bIsAttacker = false;
};
