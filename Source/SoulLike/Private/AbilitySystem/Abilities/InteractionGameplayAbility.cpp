// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/InteractionGameplayAbility.h"
#include "AbilitySystem/Task/AbilityTask_Interaction.h"
#include "Abilitysystem/Task/AbilityTask_Ladder.h"

#include "Interface/InteractionInterface.h"

#include "SoulLikeGameplayTags.h"


UInteractionGameplayAbility::UInteractionGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UInteractionGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	if(GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
	{
		InteractionTaskInfo = IInteractionInterface::Execute_GetInteractionActorInfo(GetAvatarActorFromActorInfo());
		ActivationOwnedTags = FGameplayTagContainer(InteractionTaskInfo.StatusTag);
	}
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UInteractionGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
	// if(InteractionTaskInfo.Montage == nullptr)
	// {
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	// 	return;
	// }

	/**
	 * AbilityTask로 Interaction을 구분해서 작동
	 */
	if(InteractionTaskInfo.InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder))
	{
		InteractionTask = UAbilityTask_Ladder::CreateLadderTask(this, GetAvatarActorFromActorInfo(), InteractionTaskInfo);
	}
	else
	{
		InteractionTask = UAbilityTask_Interaction::CreateInteractionTask(this, GetAvatarActorFromActorInfo(), InteractionTaskInfo);
	}

	// 시작 Montage가 종료됬을 때 Callback하는 lambda
	// InteractionTask->OnFinishedStartMontage.BindLambda([this]()->void
	// {
	//
	// });

	//InteractionTask가 종료되었을 때 작동하는 Callback
	InteractionTask->OnFinishedTask.BindLambda([this]()->void
	{
		K2_EndAbility();
	});
	InteractionTask->ReadyForActivation();

	//캐릭터를 Interaction 시킴
	if(GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interaction(GetAvatarActorFromActorInfo(), nullptr);
	}
}

void UInteractionGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(InteractionTask))
	{
		InteractionTask->EndTask();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}