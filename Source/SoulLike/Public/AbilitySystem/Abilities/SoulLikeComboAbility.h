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
 * 공격중 추가 입력으로 콤보 공격을 하게하는 Ability
 * 몽타주에 Event를 보내는 Notify를 직접 설정해야 작동함
 * 1. WaitInputEvent
 *  선 입력을 받기 시작하는 시기를 결정
 * 2. NextAction
 *  선 입력을 처리하는 시기를 결정
 * 선입력, 추가입력으로 받는 InputTag에 따라 해당 콤보의 다음 액션을 실행할 지, 다른 액션을 실행할지 결정됨
 * ex) NextInput이전에 구르기 액션에 해당하는 InputTag가 선입력 됬다면, NextInput시기에 구르기 액션 실행
 *
 * TODO
 * Avatar의 입력을 받아 Ability가 취소되지만 선입력의 한정이고
 * NextAction이후에 발생한 입력은 AnimBlend로 처리됨
 * 델리게이트로 Bind 해야할지 고민
 */
UCLASS()
class SOULLIKE_API USoulLikeComboAbility : public USoulLikeDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/**
	 * 현재 어빌리티의 상태를 초기화
	 */
	void ResetAbilityState();

	/**
	 * WaitInputEvent를 받았을 때 작동하는 Callback
	 */
	UFUNCTION()
	void ReceiveWaitInputEvent(FGameplayEventData Payload);

	/**
	 * NextActionEvent를 받았을 때 작동하는 Callback
	 */
	UFUNCTION()
	void ReceiveNextActionEvent(FGameplayEventData Payload);

	/**
	 * 해당 Ability의 InputTag를 받았을 때 작동하는 Callback
	 */
	UFUNCTION()
	void ReceiveInputPress(float TimeWaited);

	/**
	 * InputTag를 받았을 때 작동하는 Callback
	 */
	UFUNCTION()
	void ReceiveInputTag(const FGameplayTag& InInputTag);

	/**
	 * 
	 */
	virtual void SetupMontage();

	/***
	 * 주어진 SectionName과 SectionIndex로 해당 Section으로 Montage를 이동시키는 함수
	 */
	virtual void MontageJumpToNextCombo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;
	
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

	/***
	 * Ability의 Avatar에 이동Input을 감지하는 함수
	 */
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
