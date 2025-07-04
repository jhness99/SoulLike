// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Riposte.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/SoulLikeCharacterBase.h"

UGA_Riposte::UGA_Riposte()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = GameplayTags.Abilities_Riposte;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);

	TriggerData.TriggerTag = GameplayTags.Abilities_Backstab;

	AbilityTriggers.Add(TriggerData);

	AbilityTags.AddTag(GameplayTags.Abilities_Riposte);
	BlockAbilitiesWithTag.AddTag(GameplayTags.Abilities_Stagger);
	
	DeathStatusTags.AddTag(GameplayTags.Status_Death);
	DeathStatusTags.AddTag(GameplayTags.Status_AI_Death);
}

void UGA_Riposte::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	bIsAttacker = TriggerEventData->Instigator == GetAvatarActorFromActorInfo();
	
	ActivationOwnedTags.Reset();

	ActivationOwnedTags.AddTag(FSoulLikeGameplayTags::Get().Status_Riposte);
	if(bIsAttacker)
	{
		ActivationOwnedTags.AddTag(FSoulLikeGameplayTags::Get().Status_Action);
	}
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UGA_Riposte::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAnimMontage* Montage = nullptr;
	FName SectionName = FName("");
	SetupMontage(TriggerEventData, Montage, SectionName);
	
	if(Montage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	VictimActor = const_cast<AActor*>(TriggerEventData->Target.Get());
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Riposte"), Montage, 1, SectionName);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Riposte::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Riposte::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Riposte::K2_EndAbility);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Riposte::K2_EndAbility);
	MontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage, nullptr, false, false);
	WaitEventTask->EventReceived.AddDynamic(this, &UGA_Riposte::ReceiveMontageEvent);
	WaitEventTask->ReadyForActivation();
}

void UGA_Riposte::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(MontageTask))
		MontageTask->EndTask();
		
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Riposte::ReceiveMontageEvent(FGameplayEventData Payload)
{
	if(Payload.EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Event_Montage_RiposteDamage))
	{
		if(!HasAuthority(&CurrentActivationInfo)) return;
		if(!bIsAttacker|| VictimActor == nullptr) return;
		if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_ApplyDamageToTarget(GetAvatarActorFromActorInfo(), VictimActor, 2.5);
		}
	}
	if(Payload.EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Event_Montage_CheckDeath))
	{
		if(!bIsAttacker)
		{
			if(GetAbilitySystemComponentFromActorInfo() &&
				GetAbilitySystemComponentFromActorInfo()->HasAnyMatchingGameplayTags(DeathStatusTags))
			{
				MontageStop();
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo() ,GetCurrentActivationInfo(), true, true);
			}
			
		}
	}
}

void UGA_Riposte::SetupMontage(const FGameplayEventData* TriggerEventData, UAnimMontage*& Montage, FName& Section) const
{
	if(TriggerEventData->Instigator == nullptr) return;
	
	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		if(TriggerEventData->EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Abilities_Riposte))
		{
			Section = FName("Riposte");
		}
		if(TriggerEventData->EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Abilities_Backstab))
		{
			Section = FName("Backstab");
		}
		if(bIsAttacker)
		{
			Section = FName("Riposte");
			Montage = ICombatInterface::Execute_GetCurrentWeaponRiposteMontage(GetAvatarActorFromActorInfo());
		}
		else
		{
			const FVector InstigatorLocation = TriggerEventData->Instigator->GetActorLocation();
			const FVector InstigatorForwardVector = TriggerEventData->Instigator->GetActorForwardVector();
			const FVector InstigatorRightVector = TriggerEventData->Instigator->GetActorRightVector();

			const FRotator InstigatorRotation = TriggerEventData->Instigator->GetActorRotation();

			if(ASoulLikeCharacterBase* AvatarActor = Cast<ASoulLikeCharacterBase>(GetAvatarActorFromActorInfo()))
			{
				AvatarActor->SetWarpingLocationAndRotation(InstigatorLocation + InstigatorForwardVector * 70.f + InstigatorRightVector * -10.f, InstigatorRotation);
			}
				
			Montage = ICombatInterface::Execute_GetRiposteReactMontage(GetAvatarActorFromActorInfo());
		}
	}
}