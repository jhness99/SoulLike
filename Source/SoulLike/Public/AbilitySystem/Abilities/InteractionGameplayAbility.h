// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulLikeGameplayAbility.h"
#include "AbilitySystem/Task/AbilityTask_Interaction.h"
#include "Interface/InteractionInterface.h"
#include "InteractionGameplayAbility.generated.h"

class UAbilityTask_Interaction;

/**
 * AvatarActor와 Overlap되어있는 InteractionActor의 정보를 사용해서 InteractionTask를 작동시키는 어빌리티
 */
UCLASS()
class SOULLIKE_API UInteractionGameplayAbility : public USoulLikeGameplayAbility
{
	GENERATED_BODY()

public:

	UInteractionGameplayAbility();

	/**
	 * 어빌리티를 활성화 하기 전에 현재 overlap된 InteractionActor의 InteractionInfo를 설정<br>
	 * 가져온 Info에서 StatusTag를 ActivationOwnedTags로 지정해서 어빌리티가 활성화 되어있을 때의 상태Tag를 부여
	 */
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
private:
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_Interaction> InteractionTask;

	/**
	 * InteractionAbility를 작동시키기 위한 정보
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta= (AllowPrivateAccess=true))
	FInteractionTaskInfo InteractionTaskInfo;
	
};
