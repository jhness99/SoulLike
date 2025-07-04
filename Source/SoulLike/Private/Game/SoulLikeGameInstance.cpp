// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoulLikeGameInstance.h"
#include "Game/ObjectPoolingSubsystem.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"

void USoulLikeGameInstance::Init()
{
	Super::Init();
	
	if (UObjectPoolingSubsystem* PoolSubsystem = GetSubsystem<UObjectPoolingSubsystem>())
	{
		PoolSubsystem->EnemyDataAssset = this->EnemyDataAssset;
		PoolSubsystem->MaxPoolSize = this->MaxPoolSize;
		PoolSubsystem->MaxMobGenerateDistance = this->MaxMobGenerateDistance;
	}
}
