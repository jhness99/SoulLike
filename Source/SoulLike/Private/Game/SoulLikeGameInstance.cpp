// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoulLikeGameInstance.h"

#include "Game/AutoSaveSubsystem.h"
#include "Game/ObjectPoolingSubsystem.h"


void USoulLikeGameInstance::Init()
{
	Super::Init();
	
	if (UObjectPoolingSubsystem* PoolSubsystem = GetSubsystem<UObjectPoolingSubsystem>())
	{
		PoolSubsystem->EnemyDataAssset = this->EnemyDataAssset;
		PoolSubsystem->MaxPoolSize = this->MaxPoolSize;
		PoolSubsystem->MaxMobGenerateDistance = this->MaxMobGenerateDistance;
	}

	if (UAutoSaveSubsystem* AutoSaveSubsystem = GetSubsystem<UAutoSaveSubsystem>())
	{
		AutoSaveSubsystem->AutoSaveFrequency = AutoSaveFrequency;
	}

	
}
