// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Task/AbilityTask_Interaction.h"
#include "AbilityTask_Ladder.generated.h"

/**
 * LadderClimb을 구현하는 InteractionTask
 */
UCLASS()
class SOULLIKE_API UAbilityTask_Ladder : public UAbilityTask_Interaction
{
	GENERATED_BODY()
	
public:
	
	static UAbilityTask_Interaction* CreateLadderTask(UGameplayAbility* OwningAbility, AActor* AvatarActor, const FInteractionTaskInfo& InteractionTaskInfo);

protected:

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
	virtual FName GetStartSectionName() override;
	virtual FName GetEndSectionName() override;

	virtual void OnCompletedStartMontage() override;
	virtual void OnCompletedEndMontage() override;

private:

	//현재 Overlap된 위치의 이동방향을 출력
	FName GetOverlapLocationName() const;

	// UFUNCTION(Server, Reliable)
	// void Server_PlayLadderLoopMontage(const FString& Direction, const FString& RL);
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void MultiCast_PlayLadderLoopMontage(const FString& Direction, const FString& RL);
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> LadderClimbMontageTask;

	//좌우반전의 상태
	UPROPERTY()
	bool bIsRight = false;

	//Input을 받는 상태
	UPROPERTY()
	bool bCanReceiveInput = false;
};



