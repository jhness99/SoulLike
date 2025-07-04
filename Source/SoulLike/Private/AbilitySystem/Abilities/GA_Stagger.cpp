// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Stagger.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_Stagger::UGA_Stagger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = GameplayTags.Abilities_Stagger_HitReact;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
	TriggerData.TriggerTag = GameplayTags.Abilities_Stagger_Groggy;
	AbilityTriggers.Add(TriggerData);

	AbilityTags.AddTag(GameplayTags.Abilities_Stagger);
	ActivationBlockedTags.AddTag(GameplayTags.Status_SuperArmor);
}

void UGA_Stagger::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo,
                              FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	ActivationOwnedTags.Reset();
	if(TriggerEventData->EventTag.MatchesTagExact(GameplayTags.Abilities_Stagger_HitReact))
	{
		ActivationOwnedTags.AddTag(GameplayTags.Status_HitReact);
	}
	else if(TriggerEventData->EventTag.MatchesTagExact(GameplayTags.Abilities_Stagger_Groggy))
	{
		ActivationOwnedTags.AddTag(GameplayTags.Status_Groggy);
	}

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UGA_Stagger::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{ 
		Montage = ICombatInterface::Execute_GetHitReactMontage(GetAvatarActorFromActorInfo());
	}
	
	if(Montage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FName SectionName = GetHitReactDirection(TriggerEventData->Instigator);
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), Montage, 1, SectionName);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Stagger::K2_EndAbility);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Stagger::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Stagger::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Stagger::K2_EndAbility);
	
	MontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Abilities_Stagger_HitReact_Trigger,  nullptr);
	WaitEventTask->EventReceived.AddDynamic(this, &UGA_Stagger::ReceiveHitReactEvent);
	WaitEventTask->ReadyForActivation();
}

void UGA_Stagger::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(MontageTask))
		MontageTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UGA_Stagger::GetHitReactDirection(const AActor* Instigator) const
{
	FName SectionName = FName("Front");

	if(Instigator == nullptr) return SectionName;
 	
	const FVector AvatarForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector InstigatorDirectionVector = (Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(AvatarForwardVector, InstigatorDirectionVector);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(AvatarForwardVector, InstigatorDirectionVector);
	
	if(CrossProduct.Z < 0.f)
	{
		Theta *= -1.f;
	}

	if(Theta < -45.f && Theta >= -135.f)
	{
		SectionName = FName("Left");
	}
	else if(Theta < -135.f || Theta >= 135.f)
	{
		SectionName = FName("Back");
	}
	else if(Theta >= 45.f && Theta <= 135.f)
	{
		SectionName = FName("Right");
	}

	return SectionName;
}

void UGA_Stagger::ReceiveHitReactEvent(FGameplayEventData Payload)
{
	if(IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	FName SectionName = GetHitReactDirection(Payload.Instigator);
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(""), Montage, 1, SectionName);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Stagger::K2_EndAbility);
	MontageTask->ReadyForActivation();
}
