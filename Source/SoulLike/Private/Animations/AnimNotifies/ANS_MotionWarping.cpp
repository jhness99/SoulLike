// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/ANS_MotionWarping.h"

#include "Interface/CombatInterface.h"

UANS_MotionWarping::UANS_MotionWarping(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFireInEditor = false;
}

void UANS_MotionWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Owner = MeshComp->GetOwner();
	
	if(MotionWarpingParams.bUpdateTick) return;
	
	SetMotionWarpingTarget();
}

void UANS_MotionWarping::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(!MotionWarpingParams.bUpdateTick) return;
	
	SetMotionWarpingTarget();
}

void UANS_MotionWarping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_MotionWarping::SetMotionWarpingTarget() const
{
	if(Owner && Owner->Implements<UCombatInterface>())
	{
		switch(MotionWarpingParams.WarpingType)
		{
		case EAnimWarpingType::EWT_LocationAndRotation:
			ICombatInterface::Execute_SetWarpingLocationAndRotation(Owner);
			break;
		case EAnimWarpingType::EWT_Location:
			ICombatInterface::Execute_SetWarpingLocation(Owner);
			break;
		case EAnimWarpingType::EWT_Rotation:
			ICombatInterface::Execute_SetWarpingRotation(Owner);
			break;
		default:
			break;
		}
	}
}
