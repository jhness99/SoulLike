// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_SetWarpTarget.h"

#include "Character/SoulLikeCharacterBase.h"
#include "Interface/CombatInterface.h"

void UAN_SetWarpTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ASoulLikeCharacterBase* Owner = Cast<ASoulLikeCharacterBase>(MeshComp->GetOwner());
	if(Owner)
	{
		Owner->SetWarpingLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}
