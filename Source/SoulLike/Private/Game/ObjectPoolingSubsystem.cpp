// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ObjectPoolingSubsystem.h"

#include "Character/SoulLikeEnemy.h"

#include "Interface/ObjectPoolingInterface.h"

#include "Character/Data/EnemyDataAsset.h"


void UObjectPoolingSubsystem::Init()
{
	EnemyObjectPool.Empty();
	EnemyObjectPoolQueue.Empty();
}

ASoulLikeEnemy* UObjectPoolingSubsystem::SpawnEnemy(AActor* SpawnerActor, const TSubclassOf<ASoulLikeEnemy>& SpawnActorClass, FName RowName)
{
	FScopeLock Lock(&PoolLock);
	
	if(SpawnerActor == nullptr) return nullptr;
	if(!SpawnActorClass->IsValidLowLevel()) return nullptr;
	
	FEnemyData* Data = EnemyDataAssset->EnemyDataTable->FindRow<FEnemyData>(RowName, FString(""), false);
	if(Data == nullptr) return nullptr;
	
 	if(EnemyObjectPool.Num() < MaxPoolSize)
	{
		FTransform SpawnTransform = SpawnerActor->GetTransform();
		
		if(UWorld* World = SpawnerActor->GetWorld())
		{
			ASoulLikeEnemy* EnemyActor = World->SpawnActorDeferred<ASoulLikeEnemy>(Data->EnemyClass, SpawnTransform, SpawnerActor);
			if(IObjectPoolingInterface* Interface = Cast<IObjectPoolingInterface>(EnemyActor))
			{
				if(Data != nullptr)
				{
					EnemyActor->Init(SpawnerActor, *Data);
				}
				EnemyActor->OnDisabledObjectDelegate.AddDynamic(this, &UObjectPoolingSubsystem::OnEnemyDisabledObject);
				EnemyActor->SpawnDefaultController();
				EnemyActor->FinishSpawning(SpawnTransform);
			
				EnemyObjectPool.Add(EnemyActor);
				return EnemyActor;
			}
		}
	}
	else
	{
 		if(!EnemyObjectPoolQueue.IsEmpty())
		{
			AActor* PooledObject  = nullptr;
			if(EnemyObjectPoolQueue.Dequeue(PooledObject))
			{
				if(ASoulLikeEnemy* EnemyActor = Cast<ASoulLikeEnemy>(PooledObject))
				{
					EnemyActor->SetActorTransform(SpawnerActor->GetTransform(), false,  nullptr, ETeleportType::ResetPhysics);
					if(Data != nullptr)
					{
						EnemyActor->Init(SpawnerActor, *Data);
					}
					EnemyActor->SpawnDefaultController();
					return EnemyActor;
				}
			}
		}
	}
	return nullptr;
}

void UObjectPoolingSubsystem::ResetEnemyPool()
{
	FScopeLock Lock(&PoolLock);
	
	for(AActor* Object : EnemyObjectPool)
	{
		if(IObjectPoolingInterface* ObjectPoolingInterface = Cast<IObjectPoolingInterface>(Object))
		{
			ObjectPoolingInterface->Disable(false);
		}
	}
	OnResetLevelDelegate.Broadcast();
}

void UObjectPoolingSubsystem::OnEnemyDisabledObject(AActor* Actor)
{
	FScopeLock Lock(&PoolLock);
	
	EnemyObjectPoolQueue.Enqueue(Actor);
}
