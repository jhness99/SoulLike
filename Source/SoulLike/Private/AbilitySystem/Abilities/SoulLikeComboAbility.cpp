// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulLikeComboAbility.h"

#include "AbilitySystem/AsyncTask/WaitInputTask.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"
#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"


USoulLikeComboAbility::USoulLikeComboAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USoulLikeComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	/**
	 * Cost와 Cooldown을 체크하고 수행
	 */
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if(AbilityTags.HasTagExact(FSoulLikeGameplayTags::Get().Abilities_Attack) && TryBackstab())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	/**
	 * 첫 Section에서 시작
	 */
	SectionIndex = 1;
	ResetAbilityState();

	/**
	 * AvatarActor가 착용한 무기의 Montage를 가져옴
	 */
	SetupMontage();
	
	if(Montage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), Montage, 1, GetSectionName());
	MontageTask->OnCompleted.AddDynamic(this, &USoulLikeComboAbility::K2_EndAbility);
	MontageTask->OnBlendOut.AddDynamic(this, &USoulLikeComboAbility::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &USoulLikeComboAbility::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &USoulLikeComboAbility::K2_EndAbility);
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
	/**
	 * Ability가 끝나고 Task를 종료해도 Montage는 여전히 작동하므로 수동으로 Montage를 종료해야함
	 */
	if(IsValid(MontageTask))
	{
		MontageStop();
		MontageTask->EndTask();
	}
	
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

FName USoulLikeComboAbility::GetSectionName()
{
	return FName(*FString::Printf(TEXT("%s_%d"), *SectionName, SectionIndex));
}

void USoulLikeComboAbility::ResetAbilityState()
{
	bNextCombo = false;
	InputTag = FGameplayTag();
	AbilityState = EAbilityState::EAS_None;
}

void USoulLikeComboAbility::ReceiveWaitInputEvent(FGameplayEventData Payload)
{	
	if(IsValid(WaitInputTask))
		WaitInputTask->EndTask();

	if(IsValid(InputPressTask))
		InputPressTask->EndTask();

	AbilityState = EAbilityState::EAS_WaitInput;
	
	WaitInputTask = UWaitInputTask::WaitInputTag(GetAbilitySystemComponentFromActorInfo());
	WaitInputTask->WaitInputTagDelegate.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputTag);

	InputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	InputPressTask->OnPress.AddDynamic(this, &USoulLikeComboAbility::ReceiveInputPress);
	InputPressTask->Activate();
}

void USoulLikeComboAbility::ReceiveNextActionEvent(FGameplayEventData Payload)
{
	AbilityState = EAbilityState::EAS_NextAction;

	if(bNextCombo)
	{
		MontageJumpToNextCombo();
	}
	else
	{
		if(CheckAvatarInput()) return;

		if(InputTag.IsValid())
		{
			EndAbility(GetCurrentAbilitySpec()->Handle, CurrentActorInfo, CurrentActivationInfo, true, true);

			if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
			{
				SL_ASC->TryActivateAbilityFromInputTag(InputTag);
			}
		}
	}
}

void USoulLikeComboAbility::ReceiveInputPress(float TimeWaited)
{
	switch(AbilityState)
	{
		case EAbilityState::EAS_NextAction:
			MontageJumpToNextCombo();
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

void USoulLikeComboAbility::SetupMontage()
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		Montage = ICombatInterface::Execute_GetCurrentWeaponAttackMontage(GetAvatarActorFromActorInfo());
	}
}

void USoulLikeComboAbility::ClearInputTask() const
{
	if(IsValid(WaitInputTask))
		WaitInputTask->EndTask();

	if(IsValid(InputPressTask))
		InputPressTask->EndTask();
}

bool USoulLikeComboAbility::TryBackstab() const
{
	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_TryBackstab(GetAvatarActorFromActorInfo());
	}
	
	return false;
}

void USoulLikeComboAbility::AddSectionIndex()
{
	SectionIndex = FMath::Clamp(++SectionIndex, 1.f, MaxSectionIndex);
}

void USoulLikeComboAbility::MontageJumpToNextCombo()
{
	if(!HasAuthority(&CurrentActivationInfo)) return;
	
	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	ClearInputTask();
	
	AddSectionIndex();
	
	//FName Section = FName(*FString::Printf(TEXT("%s_%d"), *SectionName, SectionIndex));
	FName Section = GetSectionName();
	
	MontageJumpToSection(Section);
	ResetAbilityState();
}

bool USoulLikeComboAbility::CheckAvatarInput()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if(Character == nullptr) return false;

	float InputVector = Character->GetCharacterMovement()->GetLastInputVector().Size2D();
	if(InputVector > 0.f)
	{
		EndAbility(GetCurrentAbilitySpec()->Handle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return true;
	}
	return false;
}
