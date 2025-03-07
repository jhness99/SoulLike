// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulLikeComboAbility.h"

#include "AbilitySystem/AsyncTask/WaitInput.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/PlayerInterface.h"

#include "SoulLikeGameplayTags.h"


void USoulLikeComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
	SectionIndex = 1;
	InitAbilityState();

	if(TestMontage == nullptr) return;
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), TestMontage);
	MontageTask->OnCompleted.AddDynamic(this, &USoulLikeComboAbility::K2_EndAbility);
	MontageTask->Activate();

	WaitInputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_WaitInput);
	WaitInputEventTask->EventReceived.AddDynamic(this, &USoulLikeComboAbility::ReceiveWaitInputEvent);
	WaitInputEventTask->Activate();
	
	NextActionEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_NextAction);
	NextActionEventTask->EventReceived.AddDynamic(this, &USoulLikeComboAbility::ReceiveNextActionEvent);
	NextActionEventTask->Activate();
}

void USoulLikeComboAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(MontageTask))
		MontageTask->EndTask();
	
	if(IsValid(InputPressTask))
		InputPressTask->EndTask();
	
	if(IsValid(WaitInputEventTask))
		WaitInputEventTask->EndTask();
	
	if(IsValid(NextActionEventTask))
		NextActionEventTask->EndTask();

	if(IsValid(WaitInputTask))
		WaitInputTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USoulLikeComboAbility::InitAbilityState()
{
	bNextCombo = false;
	InputTag = FGameplayTag();
	AbilityState = EAbilityState::EAS_None;
}

void USoulLikeComboAbility::ReceiveWaitInputEvent(FGameplayEventData Payload)
{
	if(IsValid(WaitInputTask))
		WaitInputTask->EndTask();

	AbilityState = EAbilityState::EAS_WaitInput;
	
	WaitInputTask = UWaitInput::WaitInputTag(GetAbilitySystemComponentFromActorInfo());
	WaitInputTask->WaitInputTagDelegate.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputTag);

	InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	InputPressTask->OnPress.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputPress);
	InputPressTask->Activate();

	AddSectionIndex();
}

void USoulLikeComboAbility::ReceiveNextActionEvent(FGameplayEventData Payload)
{
	AbilityState = EAbilityState::EAS_NextAction;

	if(bNextCombo)
	{
		MontageSection();
	}
	else
	{
		if(CheckAvatarInput()) EndAbility(GetCurrentAbilitySpec()->Handle, CurrentActorInfo, CurrentActivationInfo, true, true);

		if(InputTag.IsValid())
		{
			EndAbility(GetCurrentAbilitySpec()->Handle, CurrentActorInfo, CurrentActivationInfo, true, true);
			if(GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
			{
				IPlayerInterface::Execute_TryActiveAbilityWithInputTag(GetAvatarActorFromActorInfo(), InputTag);
			}
		}
	}
}

void USoulLikeComboAbility::ReceiveInputPress(float TimeWaited)
{
	switch(AbilityState)
	{
		case EAbilityState::EAS_NextAction:
			MontageSection();
			break;
	case EAbilityState::EAS_WaitInput:
			bNextCombo = true;
			break;
		default:
			break;
	}
}

void USoulLikeComboAbility::ReceiveInputTag(const FGameplayTag& InInputTag)
{
	InputTag = InInputTag;
}

void USoulLikeComboAbility::AddSectionIndex()
{
	SectionIndex = FMath::Clamp(++SectionIndex, 1.f, MaxSectionIndex);
}

void USoulLikeComboAbility::MontageSection()
{
	FName Section = FName(*FString::Printf(TEXT("%s_%d"), *SectionName, SectionIndex));
	MontageJumpToSection(Section);
	InitAbilityState();
}

bool USoulLikeComboAbility::CheckAvatarInput() const
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if(Character == nullptr) return false;

	float InputVector = Character->GetCharacterMovement()->GetLastInputVector().Size2D();
	if(InputVector > 0.f) return true;

	return false;
}
