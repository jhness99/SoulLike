// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTask/WaitInput.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"

UWaitInput* UWaitInput::WaitInputTag(UAbilitySystemComponent* ASC)
{
	UWaitInput* WaitInput = NewObject<UWaitInput>();
	WaitInput->ASC = CastChecked<USoulLikeAbilitySystemComponent>(ASC);

	WaitInput->ASCWaitInputDelegateHandle = WaitInput->ASC->WaitInputDelegate.AddUObject(WaitInput, &UWaitInput::InputTag);
	
	return WaitInput;
}

void UWaitInput::EndTask()
{
	if(!IsValid(ASC)) return;
	ASC->WaitInputDelegate.Remove(ASCWaitInputDelegateHandle);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitInput::InputTag(const FGameplayTag& InputTag)
{
	WaitInputTagDelegate.Broadcast(InputTag);
}
