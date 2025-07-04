// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Abilities/TargetLockGameplayAbility.h"
#include "AbilityTask_TargetLock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedTargetActorSignature, AActor*, NewTargetActor);

USTRUCT()
struct FTargetLockTaskProperties
{
	GENERATED_BODY()

	FTargetLockTaskProperties() {}

	FTargetLockTaskProperties(FTraceProperties InTraceProperties, float InLockRetentionTimeInvisible, float InMaxDistance) :
		TraceProperties(InTraceProperties), LockRetentionTimeInvisible(InLockRetentionTimeInvisible), MaxDistance(InMaxDistance)
	{
		
	}
	
	UPROPERTY()
	FTraceProperties TraceProperties;

	UPROPERTY()
	float LockRetentionTimeInvisible = 1.f;

	UPROPERTY()
	float MaxDistance = 5000.f;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UAbilityTask_TargetLock : public UAbilityTask
{
	GENERATED_BODY()

public:

	static UAbilityTask_TargetLock* CreateTargetLockTask(UGameplayAbility* OwningAbility, AActor* TargetActor, const FTargetLockTaskProperties& TargetLockTaskProperties);

	FOnChangedTargetActorSignature ChangedTargetActorDelegate;
	
protected:

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	virtual void TickTask(float DeltaTime) override;

private:

	UFUNCTION()
	void OnDeathTargetActor();
	void LockOn();
	void TryChangeTargetActor();
	void UpdateControllRotationToTarget() const;
	
	FTimerHandle LockOnTimerHandle;
	FTimerHandle TargetDeathTimerHandle;
	
	UPROPERTY()
	FTraceProperties TraceProperties;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;
	
	UPROPERTY()
	float LockRetentionTimeInvisible = 1.f;

	UPROPERTY()
	float MaxDistance = 5000.f;

	UPROPERTY()
	bool bCanChangeTarget = false;
};