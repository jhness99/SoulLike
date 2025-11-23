// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoulLikeGameModeBase.h"

#include "Game/SoulLikeGameInstance.h"
#include "Game/SoulLikeSaveGame.h"

#include "Interface/SaveInterface.h"

#include "EngineUtils.h"
#include "Game/OnlineSessionSubsystem.h"
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

	SoulLikeSaveGame->ProfileName = PlayerName;
	
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

void ASoulLikeGameModeBase::SaveWorldObject(UWorld* World, USoulLikeSaveGame* SaveGame) const
{
	if(World == nullptr) return;
	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	check(SL_GameInstance);
	
	if(SaveGame)
	{
		TArray<FSavedActor> SavedActors;
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
			
			{
				FMemoryWriter MemoryWriter(SavedActor.Bytes);
	
				FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
				Archive.ArIsSaveGame = true;
	
				Actor->Serialize(Archive);
			}
			
			SaveGame->SavedActorsMap.Add(SavedActor.ActorName, SavedActor);
		}
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
			
			if(const FSavedActor* FoundData = SaveGame->SavedActorsMap.Find(Actor->GetFName()))
			{
				ISaveInterface::Execute_SetIsUsedObject(Actor, FoundData->bIsUsedObject);

				FMemoryReader MemoryReader(FoundData->Bytes);
				FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
				Archive.ArIsSaveGame = true;
				Actor->Serialize(Archive);

				ISaveInterface::Execute_LoadActor(Actor);
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

	SoulLikeGameInstance->SetClientSaveData(SaveObject);

	const FString InGameLoadSlotName = FString::Printf(TEXT("%s_%d"), *SoulLikeGameInstance->LoadSlotName, SoulLikeGameInstance->LoadSlotIndex);
	const int32 InGameLoadSlotIndex = SoulLikeGameInstance->LoadSlotIndex;

	SaveWorldObject(GetWorld(), SaveObject);
	
	UGameplayStatics::AsyncSaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
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
	FString LongPackageName = GameMap.ToString();
	
	FString MapPath;
	LongPackageName.Split(TEXT("."), &MapPath, nullptr);
	
	FString TravelURL = FString::Printf(TEXT("%s?listen"), *MapPath);
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting to ServerTravel to: %s"), *TravelURL);
	
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(TravelURL);
	}
}

void ASoulLikeGameModeBase::OpenSession(int32 SlotIndex)
{
	if(UOnlineSessionSubsystem* OnlineSubsystem = GetGameInstance()->GetSubsystem<UOnlineSessionSubsystem>())
	{
		OnlineSubsystem->HostSession(4, FString());
	}
}

void ASoulLikeGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if(NewPlayer->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Listen Server Controller"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client Controller"));

		APlayerController* PC = Cast<APlayerController>(NewPlayer);
		if(PC == nullptr) return;
		
		APlayerState* PS = PC->GetPlayerState<APlayerState>();
		if(PS == nullptr) return;

		TSharedPtr<const FUniqueNetId> LocalUserId = PS->GetUniqueId().GetUniqueNetId();
		FString SteamUID = LocalUserId.Get()->ToString();
		UE_LOG(LogTemp, Warning, TEXT("SteamUID : %s"), *SteamUID);

		UOnlineSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UOnlineSessionSubsystem>();
		if(SessionSubsystem == nullptr) return;

		const FVector& Location = SessionSubsystem->GetLocationWithSteamUID(SteamUID);
		UE_LOG(LogTemp, Warning, TEXT("SpawnTransform : %s"), *Location.ToString());

		if(Location == FVector::ZeroVector)
		{
			Super::RestartPlayer(NewPlayer);
			return;
		}
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SessionSubsystem->GetLocationWithSteamUID(SteamUID));
		
		RestartPlayerAtTransform(NewPlayer, SpawnTransform);
	}

	Super::RestartPlayer(NewPlayer);
}
