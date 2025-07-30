// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TargetLockGameplayAbility.h"
#include "AbilitySystem/Task/AbilityTask_TargetLock.h"
#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeFunctionLibrary.h"
#include "SoulLikeGameplayTags.h"

#include "SoulLike/SoulLike.h"


UTargetLockGameplayAbility::UTargetLockGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	
	StartupInputTag = GameplayTags.InputTag_Q;
	
	AbilityTags.AddTag(GameplayTags.Abilities_TargetLock);
	ActivationOwnedTags.AddTag(GameplayTags.Status_TargetLock);

	bIsToggleAbility = true;
}

bool UTargetLockGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UTargetLockGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//클라이언트에서만 실행
	if(!IsLocallyControlled()) return;
	
	const bool bDebug = static_cast<bool>(CVarShowTargetLockTrace.GetValueOnAnyThread());
	
	FHitResult AimHitResult;
	FTraceProperties AnimTraceProperties(TraceLength, SingleTraceCollisionChannel, SingleTraceSphereRadius, DebugLifeTime);
	FTraceProperties MultiTraceProperties(TraceLength, MultiTraceCollisionChannel, MultiTraceSphereRadius, DebugLifeTime);

	//카메라 각도(애임)을 기준으로 한 Trace 시도
	if(USoulLikeFunctionLibrary::SingleTraceFromCameraLocation(GetAvatarActorFromActorInfo(), AimHitResult, AnimTraceProperties, bDebug))
	{
		//Trace 성공
		bIsTargetLock = true;
		if(AimHitResult.GetActor())
		{
			//타겟 지정
			TargetActor = AimHitResult.GetActor();
		}
	}
	else
	{
		//Trace 실패
		TArray<FHitResult> TargetHitResults;

		//카메라 각도(애임)을 기준으로 MultiTrace 시도
		if(USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(GetAvatarActorFromActorInfo(), TargetHitResults, MultiTraceProperties, bDebug))
		{
			float NearestDistance = 5000.f;
			
			for(const FHitResult& TargetHitResult : TargetHitResults)
			{
				AActor* CurrentTargetActor = TargetHitResult.GetActor();
				if(CurrentTargetActor == nullptr) return;

				//캐릭터와의 거리가 가장 가까운 Enemy 타겟으로 지정
				float DistanceToTarget = FVector::Distance(GetAvatarActorFromActorInfo()->GetActorLocation(), CurrentTargetActor->GetActorLocation());

				if(DistanceToTarget < NearestDistance)
				{
					NearestDistance = DistanceToTarget;
					TargetActor = CurrentTargetActor;
					bIsTargetLock = true;
				}
			}
		}
	}

	if(bIsTargetLock)
	{
		if(TargetActor.IsValid())
			UE_LOG(LogTemp, Warning, TEXT("TargetLock Actor is %s"), *TargetActor->GetName())

		if(TargetActor.Get() && TargetActor->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
		}

		if(GetAvatarActorFromActorInfo() &&  GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_SetStrafeMode(GetAvatarActorFromActorInfo(), true);
		}
		
		TargetLockTask = UAbilityTask_TargetLock::CreateTargetLockTask(this, TargetActor.Get(), FTargetLockTaskProperties(MultiTraceProperties, LockRetentionTimeInvisible, MaxDistance));
		TargetLockTask->ChangedTargetActorDelegate.AddDynamic(this, &UTargetLockGameplayAbility::OnChangedTargetActor);
		TargetLockTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UTargetLockGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(TargetLockTask))
	{
		TargetLockTask->EndTask();
	}

	if(TargetActor.IsValid() && TargetActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
		TargetActor = nullptr;
	}

	if(GetAvatarActorFromActorInfo() &&  GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetStrafeMode(GetAvatarActorFromActorInfo(), false);
	}
	
	bIsTargetLock = false;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTargetLockGameplayAbility::OnChangedTargetActor(AActor* NewTargetActor)
{
	if(NewTargetActor == nullptr)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		TargetActor = NewTargetActor;
	}
}
