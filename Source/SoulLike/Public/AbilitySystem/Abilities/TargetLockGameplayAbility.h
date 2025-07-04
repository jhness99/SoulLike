// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulLikeGameplayAbility.h"
#include "SoulLike/SoulLike.h"
#include "TargetLockGameplayAbility.generated.h"

class UAbilityTask_WaitInputRelease;
class UAbilityTask_WaitInputPress;
class UAbilityTask_TargetLock;

USTRUCT(BlueprintType)
struct FTraceProperties
{
	GENERATED_BODY()

public:
	FTraceProperties(){}

	FTraceProperties(float InTraceLength, TEnumAsByte<ECollisionChannel> InTraceCollisionChannel, float InTraceSphereRadius, float InDebugLifeTime) :
		TraceLength(InTraceLength), TraceCollisionChannel(InTraceCollisionChannel), TraceSphereRadius(InTraceSphereRadius), DebugLifeTime(InDebugLifeTime)
	{
		
	}
	
	UPROPERTY()
	float TraceLength = 3000.f;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel = ECC_Visibility;
	
	UPROPERTY()
	float TraceSphereRadius = 5.f;

	UPROPERTY()
	float DebugLifeTime = 5.f;
	
	
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UTargetLockGameplayAbility : public USoulLikeGameplayAbility
{
	GENERATED_BODY()

public:
	
	UTargetLockGameplayAbility();
	
protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UFUNCTION()
	void OnChangedTargetActor(AActor* NewTargetActor);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Single Trace Property", meta=(AllowPrivateAccess=true))
	float TraceLength = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Single Trace Property",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ECollisionChannel> SingleTraceCollisionChannel = ECC_Visibility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Single Trace Property",meta=(AllowPrivateAccess=true))
	float SingleTraceSphereRadius = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multi Trace Property",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ECollisionChannel> MultiTraceCollisionChannel = ECC_TargetTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multi Trace Property",meta=(AllowPrivateAccess=true))
	float MultiTraceSphereRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dibug Property",meta=(AllowPrivateAccess=true))
	float DebugLifeTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TargetLockTask Property",meta=(AllowPrivateAccess=true))
	float LockRetentionTimeInvisible = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TargetLockTask Property",meta=(AllowPrivateAccess=true))
	float MaxDistance = 5000.f;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_TargetLock> TargetLockTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputPress> WaitInputPressTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputRelease> WaitInputReleaseTask;
	
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY()
	bool bIsTargetLock = false;
};
