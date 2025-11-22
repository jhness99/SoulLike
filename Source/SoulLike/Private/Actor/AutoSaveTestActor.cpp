// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AutoSaveTestActor.h"

AAutoSaveTestActor::AAutoSaveTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

bool AAutoSaveTestActor::GetIsUsedObject_Implementation() const
{
	return ISaveInterface::GetIsUsedObject_Implementation();
}

bool AAutoSaveTestActor::IsDirty() const
{
	return bIsDirty;
}

void AAutoSaveTestActor::MarkAsDirty()
{
	
}

void AAutoSaveTestActor::MarkAsClean()
{
	
}

