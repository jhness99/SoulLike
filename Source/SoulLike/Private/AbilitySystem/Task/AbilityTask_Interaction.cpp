// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/AbilityTask_Interaction.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


UAbilityTask_Interaction* UAbilityTask_Interaction::CreateInteractionTask(UGameplayAbility* OwningAbility,
                                                                          AActor* InAvatarActor, const FInteractionTaskInfo& InteractionTaskInfo)
{
	UAbilityTask_Interaction* InteractionTask = NewAbilityTask<UAbilityTask_Interaction>(OwningAbility);

	InteractionTask->InteractionTaskInfo = InteractionTaskInfo;
	InteractionTask->bTickingTask = false;
	
	return InteractionTask;
}


void UAbilityTask_Interaction::Activate()
{
	Super::Activate();
	
	FName Section = GetStartSectionName();
	PlayMontage(Section);
	
	if(IsValid(MontageTask))
	{
		MontageTask->OnCompleted.AddDynamic(this, &UAbilityTask_Interaction::OnCompletedStartMontage);
	}

	WaitEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, InteractionTaskInfo.InteractionTag, nullptr, true, false);
	WaitEndTask->EventReceived.AddDynamic(this, &UAbilityTask_Interaction::OnReceiveEndTask);
	WaitEndTask->ReadyForActivation();
}

void UAbilityTask_Interaction::OnDestroy(bool bInOwnerFinished)
{
	if(IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	if(IsValid(WaitEndTask))
	{
		WaitEndTask->EndTask();
	}

	if(OnFinishedTask.IsBound())
	{
		OnFinishedTask.Unbind();
	}

	if(OnFinishedStartMontage.IsBound())
	{
		OnFinishedStartMontage.Unbind();
	}
	Super::OnDestroy(bInOwnerFinished);
}

FName UAbilityTask_Interaction::GetStartSectionName()
{
	return FName(*FString::Printf(TEXT("%sStart"), *InteractionTaskInfo.SectionName));
}

FName UAbilityTask_Interaction::GetEndSectionName()
{
	return FName(*FString::Printf(TEXT("%sEnd"), *InteractionTaskInfo.SectionName));
}

void UAbilityTask_Interaction::OnCompletedStartMontage()
{
	if(OnFinishedStartMontage.IsBound())
	{
		OnFinishedStartMontage.Execute();
	}
	
	if(InteractionTaskInfo.bNotEndMontage)
	{
		if(OnFinishedTask.IsBound())
		{
			OnFinishedTask.Execute();
			return;
		}
	}

	if(bReceivedTaskEndMessage)
	{
		FGameplayEventData Payload;
		OnReceiveEndTask(Payload);
	}
}

void UAbilityTask_Interaction::OnCompletedEndMontage()
{
	if(OnFinishedTask.IsBound())
	{
		OnFinishedTask.Execute();
	}
	
}

void UAbilityTask_Interaction::OnReceiveEndTask(FGameplayEventData Payload)
{
	if(!bReceivedTaskEndMessage && MontageTask && MontageTask->IsActive())
	{
		bReceivedTaskEndMessage = true;
		return;
	}
	
	FName Section = GetEndSectionName();
	PlayMontage(Section);

	if(IsValid(MontageTask))
	{
		InteractionTaskInfo.Montage->BlendOut.SetBlendTime(0.2f);
		MontageTask->OnBlendOut.AddDynamic(this, &UAbilityTask_Interaction::OnCompletedEndMontage);
	}
}

void UAbilityTask_Interaction::PlayMontage(FName Section)
{
	if(IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, FName("InteractionMontageTask"), InteractionTaskInfo.Montage, 1.0, Section);
	MontageTask->ReadyForActivation();
}
