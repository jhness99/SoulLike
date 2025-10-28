// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/UsingToolGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Interface/CombatInterface.h"
#include "Inventory/EquipmentItemInstance.h"
#include "Inventory/RegisterableItemInstance.h"

void UUsingToolGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAnimMontage* Montage = nullptr;

	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		Montage = ICombatInterface::Execute_GetCurrentToolMontage(GetAvatarActorFromActorInfo());
		ICombatInterface::Execute_SwitchWalkSpeed(GetAvatarActorFromActorInfo());
		
		UEquipmentItemInstance* CurrentTool = ICombatInterface::Execute_GetCurrentTool(GetAvatarActorFromActorInfo());
		if(CurrentTool && CurrentTool->GetItemNum() == 0)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	
			return;
		}
	}

	if(TriggerEventData != nullptr)
	{
		ToolItemInstance = Cast<URegisterableItemInstance>(const_cast<UObject*>(TriggerEventData->OptionalObject.Get()));
		Montage = ToolItemInstance->GetUsingMontage();
	}

	if(Montage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), Montage, 1, FName(""));
	MontageTask->OnCompleted.AddDynamic(this, &UUsingToolGameplayAbility::K2_EndAbility);
	MontageTask->OnBlendOut.AddDynamic(this, &UUsingToolGameplayAbility::K2_EndAbility);
	MontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_UsingTool);
	WaitEventTask->EventReceived.AddDynamic(this, &UUsingToolGameplayAbility::ReceiveEvent);
	WaitEventTask->ReadyForActivation();
}

void UUsingToolGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SwitchRunSpeed(GetAvatarActorFromActorInfo());
	}
	
	if(IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUsingToolGameplayAbility::ReceiveEvent(FGameplayEventData Payload)
{
	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_UsingTool(GetAvatarActorFromActorInfo(), ToolItemInstance);
	}
}
