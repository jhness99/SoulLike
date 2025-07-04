// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EnemyAttackGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Character/SoulLikeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UEnemyAttackGameplayAbility::UEnemyAttackGameplayAbility()
{
	
}

void UEnemyAttackGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASoulLikeEnemy* AvatarActor = Cast<ASoulLikeEnemy>(GetAvatarActorFromActorInfo());
	if(AvatarActor == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FAttackMontageInfo AttackMontageInfo = AvatarActor->GetRandomAttackMontage();
	
	UAnimMontage* Montage = AttackMontageInfo.AttackMontages;
	if(Montage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if(GetAbilitySystemComponentFromActorInfo() && SuperArmorEffectClass->IsValidLowLevel() && AttackMontageInfo.bIsSuperArmor)
	{
		FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(SuperArmorEffectClass, 1.f, ContextHandle);
		
		SuperArmorActiveGEHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(), Montage);
	MontageTask->OnCompleted.AddDynamic(this, &UEnemyAttackGameplayAbility::K2_EndAbility);
	MontageTask->OnBlendOut.AddDynamic(this, &UEnemyAttackGameplayAbility::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UEnemyAttackGameplayAbility::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UEnemyAttackGameplayAbility::K2_EndAbility);
	MontageTask->ReadyForActivation();
}

void UEnemyAttackGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	if(SuperArmorActiveGEHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(SuperArmorActiveGEHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, bWasCancelled);
}
