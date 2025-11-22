// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_MontageEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UAN_MontageEvent::UAN_MontageEvent()
{
#if WITH_EDITOR	
	bShouldFireInEditor = false;
#endif
}

void UAN_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Payload);
}
