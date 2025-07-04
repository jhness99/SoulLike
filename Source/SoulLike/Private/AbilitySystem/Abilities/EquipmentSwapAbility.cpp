// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EquipmentSwapAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"


void UEquipmentSwapAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	
	FName SectionName = FName("");
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		if(Montage == nullptr)
		{
			ICombatInterface::Execute_NextSlot(GetAvatarActorFromActorInfo(), SlotTag);
			InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
			InputReleaseTask->OnRelease.AddDynamic(this, &UEquipmentSwapAbility::OnReleased);
			InputReleaseTask->Activate();
			return;
		}
		
 		SectionName = ICombatInterface::Execute_GetCurrentWeapon(GetAvatarActorFromActorInfo()) != nullptr ? FName("SwapWeapon") : FName("NewWeapon");
		ICombatInterface::Execute_SetMirror(GetAvatarActorFromActorInfo(), Mirror);
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, SectionName, Montage);
	MontageTask->OnCompleted.AddDynamic(this, &UEquipmentSwapAbility::K2_EndAbility);
	MontageTask->Activate();

	WeaponSwapTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_SwapWeapon, nullptr, true);
	WeaponSwapTask->EventReceived.AddDynamic(this, &UEquipmentSwapAbility::SwapWeapon);
	WeaponSwapTask->Activate();
}

void UEquipmentSwapAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetMirror(GetAvatarActorFromActorInfo(), false);
	}
	
	if(IsValid(MontageTask))
	{
		MontageStop();
		MontageTask->EndTask();
	}
	
	if(IsValid(WeaponSwapTask))
		WeaponSwapTask->EndTask();

	if(IsValid(InputReleaseTask))
		InputReleaseTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEquipmentSwapAbility::SwapWeapon(FGameplayEventData Payload)
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_NextSlot(GetAvatarActorFromActorInfo(), SlotTag);
	}
}

void UEquipmentSwapAbility::OnReleased(float TimeHeld)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}
