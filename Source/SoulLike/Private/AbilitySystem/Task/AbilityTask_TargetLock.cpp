// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/AbilityTask_TargetLock.h"
#include "AbilitySystemComponent.h"

#include "Character/SoulLikeCharacter.h"

#include "Camera/CameraComponent.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeFunctionLibrary.h"

UAbilityTask_TargetLock* UAbilityTask_TargetLock::CreateTargetLockTask(
	UGameplayAbility* OwningAbility, AActor* TargetActor, const FTargetLockTaskProperties& TargetLockTaskProperties)
{
	UAbilityTask_TargetLock* TargetLockTask = NewAbilityTask<UAbilityTask_TargetLock>(OwningAbility);

	TargetLockTask->TraceProperties = TargetLockTaskProperties.TraceProperties;
	TargetLockTask->TargetActor = TargetActor;
	TargetLockTask->LockRetentionTimeInvisible = TargetLockTaskProperties.LockRetentionTimeInvisible;
	TargetLockTask->MaxDistance = TargetLockTaskProperties.MaxDistance;
	TargetLockTask->bCanChangeTarget = true;
	TargetLockTask->bTickingTask = true;
	
	return TargetLockTask;
}

void UAbilityTask_TargetLock::Activate()
{
	Super::Activate();

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAbilityTask_TargetLock::OnDeathTargetActor);
	}
}

void UAbilityTask_TargetLock::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
	{
		CombatInterface->GetOnDeathDelegate().RemoveAll(this);
	}
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(TargetDeathTimerHandle);
	}
	
}

void UAbilityTask_TargetLock::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	LockOn();
}

void UAbilityTask_TargetLock::UpdateControllRotationToTarget() const
{
	ASoulLikeCharacter* Character = Cast<ASoulLikeCharacter>(GetAvatarActor());
	if(Character == nullptr) return;

	if(Character->GetAbilitySystemComponent() &&
		Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Rolling)) return;

	UCameraComponent* Camera = Character->GetPlayerCameraComponent();
	if(Camera == nullptr) return;

	FRotator LookAtTargetRotator = FRotationMatrix::MakeFromX(TargetActor->GetActorLocation() - Camera->GetComponentLocation()).Rotator();
		
	LookAtTargetRotator.Pitch = FMath::Clamp(LookAtTargetRotator.Pitch - 5, -35.f, 360.f);
	
	Character->SetControllerRotation(LookAtTargetRotator);
}

void UAbilityTask_TargetLock::OnDeathTargetActor()
{
	GetWorld()->GetTimerManager().ClearTimer(TargetDeathTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TargetDeathTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		
		TArray<FHitResult> TargetHitResults;
		AActor* NewTargetActor = nullptr;
		const bool bDebug = static_cast<bool>(CVarShowTargetLockTrace.GetValueOnAnyThread());
		if(USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(GetAvatarActor(), TargetHitResults, TraceProperties, bDebug))
		{
			float NearestDistance = 5000.f;

			for(const FHitResult& TargetHitResult : TargetHitResults)
			{
				AActor* CurrentTargetActor = TargetHitResult.GetActor();
				if(CurrentTargetActor == nullptr) return;

				float DistanceToTarget = FVector::Distance(GetAvatarActor()->GetActorLocation(), CurrentTargetActor->GetActorLocation());

				if(DistanceToTarget < NearestDistance)
				{
					NearestDistance = DistanceToTarget;
					NewTargetActor = CurrentTargetActor;
				}
			}
		}

		if(NewTargetActor && TargetActor.Get())
		{
			ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
			TargetActor = NewTargetActor;
			ICombatInterface::Execute_ToggleTargetMark(NewTargetActor);
			if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAbilityTask_TargetLock::OnDeathTargetActor);
			}
		}
		ChangedTargetActorDelegate.Broadcast(NewTargetActor);
	}), 1.f, false);
}

void UAbilityTask_TargetLock::LockOn()
{
	if(TargetActor.IsValid() &&
		(!USoulLikeFunctionLibrary::CheckTargetOnViewport(TargetActor.Get(), LockRetentionTimeInvisible) ||
			!USoulLikeFunctionLibrary::CheckDistanceToTarget(GetAvatarActor(), TargetActor.Get(), MaxDistance)) )
	{	
		ChangedTargetActorDelegate.Broadcast(nullptr);
		TargetActor = nullptr;
		return;
	}
	if(TargetActor.IsValid())
	{
		TryChangeTargetActor();

		UpdateControllRotationToTarget();
	}
}

void UAbilityTask_TargetLock::TryChangeTargetActor()
{
	if(TargetActor.IsValid() && !USoulLikeFunctionLibrary::CheckTargetOnViewport(TargetActor.Get(), 0.2f)) return;
	if(!bCanChangeTarget) return;
	
	bool LeftTrace = false;
	bool RightTrace = false;
	
	float NextTargetDegree = 0.f;

	ASoulLikeCharacter* Character = Cast<ASoulLikeCharacter>(GetAvatarActor());
	if(Character == nullptr) return;


	UCameraComponent* Camera = Character->GetPlayerCameraComponent();
	if(Camera == nullptr) return;
	
	if(Character->GetMouseXInput() > 8.f)
	{
		RightTrace = true;
		NextTargetDegree = 360.f;
	}
	else if(Character->GetMouseXInput() < -8.f)
	{
		LeftTrace = true;
		NextTargetDegree = -360.f;
	}

	if(!LeftTrace && !RightTrace) return;

	AActor* NewTargetActor = nullptr;
	TArray<FHitResult> HitResults;

	const bool bDebug = static_cast<bool>(CVarShowTargetLockTrace.GetValueOnAnyThread());
	USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(Character, HitResults, TraceProperties, bDebug);

	for(const FHitResult& HitResult : HitResults)
	{
		AActor* CurrentTargetActor = HitResult.GetActor();
		if(CurrentTargetActor == nullptr) continue;
		
		float LookAtTargetRotatorYaw = FRotationMatrix::MakeFromX(CurrentTargetActor->GetActorLocation() - Camera->GetComponentLocation()).Rotator().Yaw;
		float CurrentTargetDegree = FRotator::ClampAxis(LookAtTargetRotatorYaw - Camera->GetComponentRotation().Yaw);

		if(bDebug)
		{
			DrawDebugString(GetWorld(), CurrentTargetActor->GetActorLocation() + FVector(0.f, 0.f, 20.f), FString::SanitizeFloat(CurrentTargetDegree), 0, FColor::White, 5.f);
		}

		if(TargetActor != CurrentTargetActor && 
			((CurrentTargetDegree > 180.f && CurrentTargetDegree > NextTargetDegree && LeftTrace)||
				(CurrentTargetDegree < 180.f && CurrentTargetDegree < NextTargetDegree && RightTrace)))
		{
			NextTargetDegree = CurrentTargetDegree;
			NewTargetActor = CurrentTargetActor;
		}
	}
	
	if(NewTargetActor != nullptr && NewTargetActor != TargetActor && NewTargetActor->Implements<UCombatInterface>() && USoulLikeFunctionLibrary::CheckTargetOnViewport(NewTargetActor, 0.2f))
	{
		ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());
		TargetActor = NewTargetActor;
		
		ChangedTargetActorDelegate.Broadcast(TargetActor.Get());
		
		ICombatInterface::Execute_ToggleTargetMark(TargetActor.Get());

		bCanChangeTarget = false;
		GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(LockOnTimerHandle, FTimerDelegate::CreateLambda([&](){
			bCanChangeTarget = true;
		}), 1.f, false);
	}
}
