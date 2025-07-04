// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SendMontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UANS_SendMontageEvent : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UANS_SendMontageEvent();

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BeginTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EndTag;
};
