// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AutoSaveSubsystem.h"

#include "Character/SoulLikeCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAutoSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAutoSaveSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().ClearTimer(AutoSaveTimer);
	}
}

void UAutoSaveSubsystem::Init()
{
	UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().SetTimer(AutoSaveTimer, this, &UAutoSaveSubsystem::HandleAutoSave, AutoSaveFrequency, true);
	}
}

void UAutoSaveSubsystem::HandleAutoSave()
{
	if(APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if(Pawn->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_SaveProgress(Pawn);
		}
	}
}