// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SaveInterface.h"
#include "SpawnPointActor.generated.h"

class ASoulLikeEnemy;
class UBoxComponent;
class USceneComponent;
class UObjectPoolingSubsystem;

UCLASS()
class SOULLIKE_API ASpawnPointActor : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASpawnPointActor();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void SpawnEnemy();

	/** Save Interface */
	bool GetIsUsedObject_Implementation() const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> EnterBossRoom;
	
private:

	UFUNCTION()
	void OnDeathEnemy();

	UFUNCTION()
	void OnResetLevel();

	UFUNCTION()
	void BeginOverlapBossRoomCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapBossRoomCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnData", meta=(AllowPrivateAccess=true))
	TSubclassOf<ASoulLikeEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnData", meta=(AllowPrivateAccess=true))
	FName EnemyId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnData", meta=(AllowPrivateAccess=true))
	bool bIsRespawn = true;
	
	UPROPERTY()
	TObjectPtr<UObjectPoolingSubsystem> ObjectPoolingSubsystem;

	UPROPERTY()
	TObjectPtr<ASoulLikeEnemy> SpawnedEnemy = nullptr;

	UPROPERTY(SaveGame)
	bool bIsDeadEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnData", meta=(AllowPrivateAccess=true))
	bool bIsBoss = false;
};
