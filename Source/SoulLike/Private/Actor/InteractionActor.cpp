// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractionActor.h"

#include "Components/BoxComponent.h"

#include "SoulLikeFunctionLibrary.h"


AInteractionActor::AInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(GetRootComponent());
}

const FInteractionTaskInfo AInteractionActor::GetInteractionActorInfo_Implementation() const
{
	return USoulLikeFunctionLibrary::GetInteractionTaskInfoForTag(this, InteractionTag);
}

bool AInteractionActor::GetIsUsedObject_Implementation() const
{
	return false;
}
