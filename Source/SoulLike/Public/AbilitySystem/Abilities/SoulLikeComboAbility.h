// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/Abilities/SoulLikeDamageGameplayAbility.h"
#include "SoulLikeComboAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class UWaitInput;

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	EAS_None UMETA(DisplayName = "None"),
	EAS_WaitInput UMETA(DisplayName = "Input Wait"),
	EAS_NextAction UMETA(DisplayName = "Next Action")
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeComboAbility : public USoulLikeDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void ResetAbilityState();
	
	UFUNCTION()
	void ReceiveWaitInputEvent(FGameplayEventData Payload);

	UFUNCTION()
	void ReceiveNextActionEvent(FGameplayEventData Payload);
	
	UFUNCTION()
	void ReceiveInputPress(float TimeWaited);

	UFUNCTION()
	void ReceiveInputTag(const FGameplayTag& InInputTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* TestMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityState AbilityState = EAbilityState::EAS_None;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxSectionIndex = 4;

	UPROPERTY(EditDefaultsOnly)
	FString SectionName = FString("");

	UPROPERTY()
	FGameplayTag InputTag;

	UPROPERTY()
	int32 SectionIndex = 1;

	UPROPERTY()
	bool bNextCombo = false;
	
private:

	void AddSectionIndex();
	void MontageJumpToNextCombo();
	bool CheckAvatarInput();

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputPress> InputPressTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitInputEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> NextActionEventTask;

	UPROPERTY()
	TObjectPtr<UWaitInput> WaitInputTask;
	
};
