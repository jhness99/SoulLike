// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ObjectPoolingSubsystem.h"

#include "Character/SoulLikeEnemy.h"

#include "Interface/ObjectPoolingInterface.h"

#include "Character/Data/EnemyDataAsset.h"


void UObjectPoolingSubsystem::Init()
{
	EmenyObjectPool.Empty();
	EnemyObjectPoolQueue.Empty();
}

ASoulLikeEnemy* UObjectPoolingSubsystem::SpawnEmeny(AActor* SpawnerActor, const TSubclassOf<ASoulLikeEnemy>& SpawnActorClass, FName RowName)
{
	FScopeLock Lock(&PoolLock);
	
	if(SpawnerActor == nullptr) return nullptr;
	if(!SpawnActorClass->IsValidLowLevel()) return nullptr;
	
	FEnemyData* Data = EnemyDataAssset->EnemyDataTable->FindRow<FEnemyData>(RowName, FString(""), false);
	if(Data == nullptr) return nullptr;
	
 	if(EmenyObjectPool.Num() < MaxPoolSize)
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
			}
			EnemyActor->OnDisabledObjectDelegate.AddDynamic(this, &UObjectPoolingSubsystem::OnEnemyDisabledObject);
			EnemyActor->SpawnDefaultController();
			EnemyActor->FinishSpawning(SpawnTransform);
			
			EmenyObjectPool.Add(EnemyActor);
			return EnemyActor;
		}
	}
	else
	{
		
 		if(!EnemyObjectPoolQueue.IsEmpty())
		{
			AActor* PooledActor  = nullptr;
			if(EnemyObjectPoolQueue.Dequeue(PooledActor))
			{
				if(ASoulLikeEnemy* EnemyActor = Cast<ASoulLikeEnemy>(PooledActor))
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

	for(ASoulLikeEnemy* Enemy : EmenyObjectPool)
	{
		if(Enemy)
			Enemy->Disable(false);
	}
	OnResetLevelDelegate.Broadcast();
}

void UObjectPoolingSubsystem::OnEnemyDisabledObject(AActor* Actor)
{
	FScopeLock Lock(&PoolLock);
	UE_LOG(LogTemp, Warning, TEXT("ActorName : %s, FunctionName : %hs"), *GetNameSafe(this), __FUNCTION__);
	EnemyObjectPoolQueue.Enqueue(Actor);
}
