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

void AInteractionActor::Interaction_Implementation(AActor* InteractionActor)
{
	MarkAsDirty();
}

bool AInteractionActor::GetIsUsedObject_Implementation() const
{
	return false;
}

bool AInteractionActor::IsDirty() const
{
	return bIsDirty;
}

void AInteractionActor::MarkAsDirty()
{
	bIsDirty = true;
}

void AInteractionActor::MarkAsClean()
{
	bIsDirty = false;
}
