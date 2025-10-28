// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OnlineSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Online/OnlineSessionNames.h"

#include "Game/SL_SubsystemSettings.h"

#include "Actor/SummonSignActor.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SoulLikePlayerState.h"


UOnlineSessionSubsystem::UOnlineSessionSubsystem()
{
}

void UOnlineSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    if(const USL_SubsystemSettings* Settings = GetDefault<USL_SubsystemSettings>())
    {
        WhiteSignActorClass = Settings->WhiteSignActorClass;
        MaxSearchSignDistance = Settings->MaxSearchSignDistance;
    }

    if ( IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        // SessionInterface = Subsystem->GetSessionInterface();
        // if (SessionInterface.IsValid())
        // {
        //     SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnCreateSessionComplete);
        //     SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnFindSessionsComplete);
        //     SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnJoinSessionComplete);
        //     SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnDestroySessionComplete);
        //
        //     UE_LOG(LogTemp, Warning, TEXT("OnlineSessionSubsystem Initialized."));
        // }

        SummonSignSessionInterface = Subsystem->GetSessionInterface();
        if (SummonSignSessionInterface.IsValid())
        {
            SummonSignSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnCreateSummonSignSessionComplete);
            SummonSignSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineSessionSubsystem::OnFindSummonSignSessionComplete);
            UE_LOG(LogTemp, Warning, TEXT("OnlineSessionSubsystem Initialized."));
        }

        IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            int32 LocalPlayerNum = 0; 
            TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayerNum);

            if (UserId.IsValid())
            {
                OnlineState.SteamUID = UserId->ToString();
                UE_LOG(LogTemp, Warning, TEXT("My Steam UID is: %s"), *OnlineState.SteamUID);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get Identity Interface."));
        }
    }
}

void UOnlineSessionSubsystem::Deinitialize()
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
        SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
        SessionInterface->ClearOnJoinSessionCompleteDelegates(this);
        SessionInterface->ClearOnDestroySessionCompleteDelegates(this);
    }

    if (SummonSignSessionInterface.IsValid())
    {
        SummonSignSessionInterface->ClearOnCreateSessionCompleteDelegates(this);
        SummonSignSessionInterface->ClearOnFindSessionsCompleteDelegates(this);
    
        FNamedOnlineSession* ExistingSession = SummonSignSessionInterface->GetNamedSession(FName("SummonSignLobby"));
        if (ExistingSession != nullptr)
        {
            SummonSignSessionInterface->DestroySession(FName("SummonSignLobby"));
        }
    }
    
    Super::Deinitialize();
}

void UOnlineSessionSubsystem::HostSession(int32 NumPublicConnections, FString MapName)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SessionInterface is not valid!"));
        return;
    }
    
    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr)
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }
    
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->NumPublicConnections = NumPublicConnections;
    SessionSettings->bIsLANMatch = false;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bUsesPresence = true;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->Set(FName("GAMENAME"), FString("SOULLIKE"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->bUseLobbiesIfAvailable = true;

    SessionInterface->CreateSession(0, NAME_GameSession, *SessionSettings);
}

void UOnlineSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully: %s"), *SessionName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session!"));
    }
}

void UOnlineSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        if (SessionSearch->SearchResults.Num() > 0)
        {
            FOnlineSessionSearchResult FirstResult = SessionSearch->SearchResults[0];
            SessionInterface->JoinSession(0, NAME_GameSession, FirstResult);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No sessions found."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find sessions."));
    }
}

void UOnlineSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        FString ConnectString;
        if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
        {
            UE_LOG(LogTemp, Warning, TEXT("Joining server with connect string: %s"), *ConnectString);

            APlayerController* PC = UGameplayStatics::GetPlayerControllerFromID(this, 0);
            if (PC)
            {
                PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to join session. Result: %d"), (int32)Result);
    }
}

void UOnlineSessionSubsystem::FindAndJoinSession()
{
    if (!SessionInterface.IsValid())
    {
        return;
    }

    // 세션 검색 객체 생성
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 10000; // 최대 검색 결과
    SessionSearch->bIsLanQuery = false; // LAN이 아닌 인터넷에서 검색
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    SessionSearch->QuerySettings.Set(FName("GAMENAME"), FString("SOULLIKE"), EOnlineComparisonOp::Equals);
    
    // 세션 검색 요청
    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UOnlineSessionSubsystem::RegistrationSummon(const FTransform& Transform)
{
    if (!SummonSignSessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SessionInterface is not valid!"));
        return;
    }
    
    auto ExistingSession = SummonSignSessionInterface->GetNamedSession(FName("SummonSignLobby"));
    if (ExistingSession != nullptr)
    {
        SummonSignSessionInterface->DestroySession(FName("SummonSignLobby"));
    }

    ASoulLikePlayerState* SL_PS = Cast<ASoulLikePlayerState>(UGameplayStatics::GetPlayerState(this, 0));
    if(SL_PS == nullptr) return;
    
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->NumPublicConnections = 0;
    SessionSettings->bIsLANMatch = false;
    SessionSettings->bAllowJoinInProgress = false;
    SessionSettings->bUsesPresence = false;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->Set(FName("GAMENAME"), FString("SOULLIKE"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set<float>(FName("LOC_X"), Transform.GetLocation().X, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set<float>(FName("LOC_Y"), Transform.GetLocation().Y, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set<float>(FName("LOC_Z"), Transform.GetLocation().Z, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set(FName("CLIENT_STEAMID"), OnlineState.SteamUID, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set(FName("PROFILE_NAME"), SL_PS->GetProfileName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings->Set<int32>(FName("PLAYER_LEVEL"), SL_PS->GetPlayerLevel(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f, Z : %f"), Transform.GetLocation().X, Transform.GetLocation().Y, Transform.GetLocation().Z);
    UE_LOG(LogTemp, Warning, TEXT("PROFILE_NAME : %s"), *SL_PS->GetProfileName());
    UE_LOG(LogTemp, Warning, TEXT("PLAYER_LEVEL : %d"), SL_PS->GetPlayerLevel());
    SessionSettings->bUseLobbiesIfAvailable = true;

    SummonSignSessionInterface->CreateSession(0, FName("SummonSignLobby"), *SessionSettings);
}

void UOnlineSessionSubsystem::FindNearBySign()
{
    if (!SummonSignSessionInterface.IsValid())
    {
        return;
    }

    // 세션 검색 객체 생성
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 10000; // 최대 검색 결과
    SessionSearch->bIsLanQuery = false; // LAN이 아닌 인터넷에서 검색
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    SessionSearch->QuerySettings.Set(FName("GAMENAME"), FString("SOULLIKE"), EOnlineComparisonOp::Equals);
    
    // 세션 검색 요청
    SummonSignSessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UOnlineSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session %s destroyed."), *SessionName.ToString());
    }
}

void UOnlineSessionSubsystem::OnCreateSummonSignSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sign Session %s Create."), *SessionName.ToString());
    }
}

void UOnlineSessionSubsystem::OnFindSummonSignSessionComplete(bool bWasSuccessful)
{
    if(APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        for(auto Pair : SteamIDToSummonSignMap)
        {
            const float Distance = (Pair.Value->GetActorLocation() - Player->GetActorLocation()).Length();

            if(MaxSearchSignDistance < Distance)
            {
                Pair.Value->Destroy();
                Pair.Value = nullptr;

                SteamIDToSummonSignMap.Remove(Pair.Key);
            }
        }
        if (bWasSuccessful && SessionSearch.IsValid())
        {
            for(const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
            {
                const FOnlineSessionSettings& SessionSettings = Result.Session.SessionSettings;

                FString SteamUID;
                SessionSettings.Get<FString>(FName("CLIENT_STEAMID"), SteamUID);

                if(SteamIDToSummonSignMap.Contains(SteamUID)) continue;
                
                float SignX;
                float SignY;
                float SignZ;
                
                bool bFoundLocationX = SessionSettings.Get<float>(FName("LOC_X"), SignX);
                bool bFoundLocationY = SessionSettings.Get<float>(FName("LOC_Y"), SignY);
                bool bFoundLocationZ = SessionSettings.Get<float>(FName("LOC_Z"), SignZ);
                
                const FVector SignLocation = FVector(SignX, SignY, SignZ);
                const FVector PlayerLoc = Player->GetActorLocation();
                const float Distance = (SignLocation - PlayerLoc).Length();

                UE_LOG(LogTemp, Warning, TEXT("Sign Distance is %f"), Distance);

                if(MaxSearchSignDistance >= Distance)
                {
                    SpawnSignObject(SessionSettings, SignLocation);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to find sign sessions."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%hs func Can't get LocalPlayer"), __FUNCTION__);
    }
}

void UOnlineSessionSubsystem::SpawnSignObject(const FOnlineSessionSettings& SessionSettings, const FVector& SpawnLocation)
{
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SpawnLocation);
    UE_LOG(LogTemp, Error, TEXT("SpawnSignObject"));
    if(ASummonSignActor* SignActor = GetWorld()->SpawnActorDeferred<ASummonSignActor>(WhiteSignActorClass, SpawnTransform))
    {
        FSummonSignState SummonSignState;
        
        bool bFoundSteamUID = SessionSettings.Get<FString>(FName("CLIENT_STEAMID"), SummonSignState.SteamUID);
        bool bFoundProfileName = SessionSettings.Get<FString>(FName("PROFILE_NAME"), SummonSignState.ProfileName);
        bool bFoundLevel = SessionSettings.Get<int32>(FName("PLAYER_LEVEL"), SummonSignState.Level);

        UE_LOG(LogTemp, Error, TEXT("SignActor spawn success"));
        UE_LOG(LogTemp, Error, TEXT("SteamUID : %s, ProfileName : %s, PLAYER_LEVEL : %d"), *SummonSignState.SteamUID, *SummonSignState.ProfileName, SummonSignState.Level);
        if(bFoundSteamUID && bFoundProfileName && bFoundLevel)
        {
            SignActor->Init(SummonSignState);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to SpawnSignObject because of SessionSettings."));
        }

        SignActor->FinishSpawning(SpawnTransform);
        SteamIDToSummonSignMap.Add(SummonSignState.SteamUID, SignActor);
    }
}

void UOnlineSessionSubsystem::StartSearchSign()
{
    if(UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(SearchSignTimerHandle, this, &UOnlineSessionSubsystem::FindNearBySign, SearchFrequency, true);
    }
}

void UOnlineSessionSubsystem::SendInviteWithSteamUID(FString SteamUID)
{
    if(SummonSignSessionInterface.IsValid())
    {
        ULocalPlayer* LocalPlayer =  UGameplayStatics::GetPlayerController(this, 0)->GetLocalPlayer();
        if (!LocalPlayer) return;

        TSharedPtr<const FUniqueNetId> LocalUserId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
        if (!LocalUserId.IsValid()) return;
        
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (!Subsystem) return;

        IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (!Identity.IsValid()) return;

        TSharedPtr<const FUniqueNetId> TargetUserId = Identity->CreateUniquePlayerId(SteamUID);
        if (!TargetUserId.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create UniqueNetId from string: %s"), *SteamUID);
        }
        
        // bool bWasSuccessful = SessionInterface->SendSessionInviteToFriend(*LocalUserId, NAME_GameSession, *TargetUserId);
        //
        // if (bWasSuccessful)
        // {
        //     UE_LOG(LogTemp, Log, TEXT("Successfully sent session invite to user %s (using SendSessionInviteToFriend)"), *SteamUID);
        // }
        // else
        // {
        //     UE_LOG(LogTemp, Warning, TEXT("Failed to send session invite to user %s (using SendSessionInviteToFriend)"), *SteamUID);
        // }
    }
}
