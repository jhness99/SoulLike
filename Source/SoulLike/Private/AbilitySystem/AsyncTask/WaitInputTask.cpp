// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTask/WaitInputTask.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"

UWaitInputTask* UWaitInputTask::WaitInputTag(UAbilitySystemComponent* ASC)
{
	UWaitInputTask* WaitInput = NewObject<UWaitInputTask>();
	WaitInput->ASC = CastChecked<USoulLikeAbilitySystemComponent>(ASC);

	WaitInput->ASCWaitInputDelegateHandle = WaitInput->ASC->WaitInputDelegate.AddUObject(WaitInput, &UWaitInputTask::InputTag);
	
	return WaitInput;
}

void UWaitInputTask::EndTask()
{
	if(!IsValid(ASC)) return;
	ASC->WaitInputDelegate.Remove(ASCWaitInputDelegateHandle);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitInputTask::InputTag(const FGameplayTag& InputTag)
{
	WaitInputTagDelegate.Broadcast(InputTag);
}
