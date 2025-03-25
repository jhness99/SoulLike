// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_SetWarpTarget.h"

#include "Interface/CombatInterface.h"

void UAN_SetWarpTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if(Owner != nullptr && Owner->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetWarpingLocationAndRotation(Owner);
	}
}
