// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Interface/InteractionInterface.h"
#include "AbilityTask_Interaction.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class ASoulLikeCharacter;

DECLARE_DELEGATE(FOnTriggerToAbility);

/**
 * InteractionAbility를 통해 작동하는 Task
 */
UCLASS()
class SOULLIKE_API UAbilityTask_Interaction : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	static UAbilityTask_Interaction* CreateInteractionTask(UGameplayAbility* OwningAbility, AActor* AvatarActor, const FInteractionTaskInfo& InteractionTaskInfo);

	//Task가 종료될 때 Callback
	FOnTriggerToAbility OnFinishedTask;
	//시작Montage가 종료될 때 Callback
	FOnTriggerToAbility OnFinishedStartMontage;

protected:
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	//시작 Montage의 SectionName을 반환
	virtual FName GetStartSectionName();
	//종료 Montage의 SectionName을 반환
	virtual FName GetEndSectionName();

	//시작 Montage가 종료될 때 Callback하는 함수
	UFUNCTION()
	virtual void OnCompletedStartMontage();

	//종료 Montage가 종료될 때 Callback하는 함수
	UFUNCTION()
	virtual void OnCompletedEndMontage();
	
	UPROPERTY()
	FInteractionTaskInfo InteractionTaskInfo;
	
private:
	
	UFUNCTION()
	void OnReceiveEndTask(FGameplayEventData Payload);
	
	void PlayMontage(FName Section);
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	//종료 Message를 대기하는 Task
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEndTask;

	//종료 Message를 Task시작전에 받았는지 여부
	UPROPERTY()
	bool bReceivedTaskEndMessage = false;
	
};
