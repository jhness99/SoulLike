// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolingSubsystem.generated.h"

class UEnemyDataAsset;
class ASoulLikeEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetLevel);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UObjectPoolingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	void Init();
	ASoulLikeEnemy* SpawnEmeny(AActor* SpawnerActor, const TSubclassOf<ASoulLikeEnemy>& SpawnActorClass, FName RowName);

	void ResetEnemyPool();

	FOnResetLevel OnResetLevelDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UEnemyDataAsset> EnemyDataAssset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EnemyObjectPoolilng")
	int32 MaxPoolSize = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EnemyObjectPoolilng")
	float MaxMobGenerateDistance = 300.f;
	
private:

	UFUNCTION()
	void OnEnemyDisabledObject(AActor* Actor);
	
	UPROPERTY()
	TArray<ASoulLikeEnemy*> EmenyObjectPool;
	
	TQueue<AActor*> EnemyObjectPoolQueue;

	FCriticalSection PoolLock;
};
