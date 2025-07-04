// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/ANS_SendMontageEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"

UANS_SendMontageEvent::UANS_SendMontageEvent()
{
	bShouldFireInEditor = false;
}

void UANS_SendMontageEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	FGameplayEventData Payload;
	Payload.EventTag = BeginTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), BeginTag, Payload);
}

void UANS_SendMontageEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	FGameplayEventData Payload;
	Payload.EventTag = EndTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EndTag, Payload);
}
