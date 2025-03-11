// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_MontageEvent.generated.h"

/**
 * Montage에서 GameplayTag를 Event화해서 보내주는 Notify
 */
UCLASS()
class SOULLIKE_API UAN_MontageEvent : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAN_MontageEvent();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;
};
