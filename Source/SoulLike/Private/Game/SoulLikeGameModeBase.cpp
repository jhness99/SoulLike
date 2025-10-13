// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoulLikeGameModeBase.h"

#include "Game/SoulLikeGameInstance.h"
#include "Game/SoulLikeSaveGame.h"

#include "Interface/SaveInterface.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SoulLikePlayerState.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


int32 ASoulLikeGameModeBase::SaveSlotData(const FString& PlayerName)
{
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	if (SoulLikeGameInstance == nullptr) return -1;
	
	FString SaveGameDir = FPaths::ProjectSavedDir() + TEXT("SaveGames/");
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> SaveFiles;
	FileManager.FindFiles(SaveFiles, *SaveGameDir, TEXT("*.sav"));
	
	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, SaveFiles.Num());
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SoulLikeSaveGameClass);
	USoulLikeSaveGame* SoulLikeSaveGame = Cast<USoulLikeSaveGame>(SaveGameObject);

	SoulLikeSaveGame->PlayerName = PlayerName;
	
	UGameplayStatics::SaveGameToSlot(SoulLikeSaveGame, InGameLoadSlotName, SaveFiles.Num());
	return SaveFiles.Num();
}

void ASoulLikeGameModeBase::DeleteSaveSlotData(int32 SlotIndex)
{
	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	check(SL_GameInstance);

	const FString SlotName = FString::Printf(TEXT("%s_%d"), *SL_GameInstance->LoadSlotName, SlotIndex);
	
	UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

void ASoulLikeGameModeBase::SaveWorldObject(UWorld* World) const
{
	if(World == nullptr) return;
	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	check(SL_GameInstance);
	
	if(USoulLikeSaveGame* SaveGame = RetrieveInGameSaveData())
	{
		for(FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
	
			if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			if(TScriptInterface<ISaveInterface> SaveInterface = Actor)
			{
				if(!SaveInterface->IsDirty() || Actor->Implements<ASoulLikePlayerState>())
				{
					continue;
				}
			}
	
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.bIsUsedObject = ISaveInterface::Execute_GetIsUsedObject(Actor);
	
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
	
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
	
			Actor->Serialize(Archive);
			
			SaveGame->SavedActors.Add(SavedActor);
		}
		
		SaveInGameProgressData(SaveGame);
	}
}

void ASoulLikeGameModeBase::LoadWorldObject(UWorld* World) const
{
	if(World == nullptr) return;
	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	check(SL_GameInstance);

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SL_GameInstance->LoadSlotName, SL_GameInstance->LoadSlotIndex);
	const int32 InGameLoadSlotIndex = SL_GameInstance->LoadSlotIndex;

	if(UGameplayStatics::DoesSaveGameExist(InGameLoadSlotName, InGameLoadSlotIndex))
	{
		USoulLikeSaveGame* SaveGame = Cast<USoulLikeSaveGame>(UGameplayStatics::LoadGameFromSlot(InGameLoadSlotName, InGameLoadSlotIndex));
		if(SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load slot"));
			return;
		}

		for(FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if(!Actor->Implements<USaveInterface>()) continue;

			for(FSavedActor SavedActor : SaveGame->SavedActors)
			{
				if(SavedActor.ActorName == Actor->GetFName())
				{
					ISaveInterface::Execute_SetIsUsedObject(Actor, SavedActor.bIsUsedObject);

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

USoulLikeSaveGame* ASoulLikeGameModeBase::GetSaveGameForIndex(int32 Index)
{
	USaveGame* SaveGameObject = nullptr;
	
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	if (SoulLikeGameInstance == nullptr) return nullptr;

	if(Index >= 8) return nullptr;

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, Index);
	
	if(UGameplayStatics::DoesSaveGameExist(InGameLoadSlotName, Index))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(InGameLoadSlotName, Index);
	}
	
	USoulLikeSaveGame* SL_SaveGame = Cast<USoulLikeSaveGame>(SaveGameObject);
	return SL_SaveGame;
}

USoulLikeSaveGame* ASoulLikeGameModeBase::RetrieveInGameSaveData() const
{
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, SoulLikeGameInstance->LoadSlotIndex);
	const int32 InGameLoadSlotIndex = SoulLikeGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ASoulLikeGameModeBase::SaveInGameProgressData(USoulLikeSaveGame *SaveObject) const
{
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, SoulLikeGameInstance->LoadSlotIndex);
	const int32 InGameLoadSlotIndex = SoulLikeGameInstance->LoadSlotIndex;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
	
}

USoulLikeSaveGame* ASoulLikeGameModeBase::GetSaveSlotData(const FString &SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if(UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SoulLikeSaveGameClass);
	}
	USoulLikeSaveGame* SoulLikeSaveGame = Cast<USoulLikeSaveGame>(SaveGameObject);
	
	return SoulLikeSaveGame;
}

void ASoulLikeGameModeBase::TravelToMap(int32 SlotIndex)
{
	if(SlotIndex == -1)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, LobbyMap);
		return;
	}
	
	USoulLikeGameInstance* SoulLikeGameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	if(SoulLikeGameInstance == nullptr) return;

	SoulLikeGameInstance->LoadSlotIndex = SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameMap);
}