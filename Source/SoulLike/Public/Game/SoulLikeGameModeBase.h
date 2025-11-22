// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoulLikeGameModeBase.generated.h"

class USoulLikeSaveGame;
class USaveGame;
/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	int32 SaveSlotData(const FString& PlayerName);
	UFUNCTION(BlueprintCallable)
	void DeleteSaveSlotData(int32 SlotIndex);

	void SaveWorldObject(UWorld* World, USoulLikeSaveGame* SaveGame = nullptr) const;
	void LoadWorldObject(UWorld* World) const;
	
	UFUNCTION(BlueprintCallable)
	USoulLikeSaveGame* GetSaveGameForIndex(int32 Index);
	USoulLikeSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameProgressData(USoulLikeSaveGame *SaveObject) const;
	USoulLikeSaveGame* GetSaveSlotData(const FString &SlotName, int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable)
	void TravelToMap(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void OpenSession(int32 SlotIndex);
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SoulLikeSaveGameClass;

protected:

	virtual void RestartPlayer(AController* NewPlayer) override;
};
