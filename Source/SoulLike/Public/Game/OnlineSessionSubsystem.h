// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSubsystem.generated.h"

class ASummonSignActor;

UENUM()
enum class ESessionState : uint8
{
	ESS_SinglePlay = 0,
	ESS_Searching,
	ESS_Join,
};

USTRUCT()
struct FOnlineState
{
	GENERATED_BODY()

	UPROPERTY()
	FString SteamUID;
	
	UPROPERTY()
	ESessionState SessionState = ESessionState::ESS_SinglePlay;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UOnlineSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UOnlineSessionSubsystem();

	// UGameInstanceSubsystem의 생명주기 함수
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// UI 등 외부에서 호출할 함수들
	UFUNCTION(BlueprintCallable)
	void HostSession(int32 NumPublicConnections, FString MapName);

	UFUNCTION(BlueprintCallable)
	void FindAndJoinSession();

	UFUNCTION(BlueprintCallable)
	void RegistrationSummon(const FTransform& Transform);

	UFUNCTION(BlueprintCallable)
	void FindNearBySign();

	UFUNCTION(BlueprintCallable)
	void StartSearchSign();
	UFUNCTION(BlueprintCallable)
	void StopSearchSign();

	UFUNCTION(BlueprintCallable)
	void CreateClientSummonSign(const TSubclassOf<ASummonSignActor>& SummonSignActorClass, const FVector& Location);
	UFUNCTION(BlueprintCallable)
	void DeleteClientSummonSign();

	void DestroySummonSignSession();

	UFUNCTION(BlueprintPure)
	bool CheckIsClientUID(const FString& UID);
	
	FORCEINLINE void SetMaxSearchSignDistance(float Distance)
	{
		MaxSearchSignDistance = Distance;
	}

	FORCEINLINE void SetWhiteSignActorClass(TSubclassOf<AActor> InActorClass)
	{
		WhiteSignActorClass = InActorClass;
	}
	
	bool SendInviteWithSteamUID(const FString& SteamUID);
	FORCEINLINE const FVector& GetLocationWithSteamUID(const FString& SteamUID)
	{
		if(SteamUIDToSpawnLocation.Contains(SteamUID))
			return SteamUIDToSpawnLocation[SteamUID];
		else
			return FVector::ZeroVector;
	}

protected:
	// 세션 인터페이스 포인터
	IOnlineSessionPtr SessionInterface;

	// 소환사인세션
	IOnlineSessionPtr SummonSignSessionInterface;

	UPROPERTY(Transient)
	FOnlineState OnlineState;

	// 세션 검색 결과 객체 포인터
	TSharedPtr<FOnlineSessionSearch> SummonSignSessionSearch;
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnCreateSummonSignSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSummonSignSessionComplete(bool bWasSuccessful);
	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);

	ASummonSignActor* SpawnSignObject(const FOnlineSessionSettings& SessionSettings, const FVector& SpawnLocation);

private:
	
	/** OnlineSessionSubsystem Properties*/
	UPROPERTY()
	TMap<FString, TObjectPtr<ASummonSignActor>> SteamUIDToSummonSignMap;

	UPROPERTY()
	TMap<FString, FVector> SteamUIDToSpawnLocation;
	
	UPROPERTY()
	TSubclassOf<AActor> WhiteSignActorClass;

	UPROPERTY()
	TObjectPtr<ASummonSignActor> ClientSummonSign;

	UPROPERTY()
	float MaxSearchSignDistance = 750.f;

	UPROPERTY()
	float SearchFrequency = 5.f;

	FTimerHandle SearchSignTimerHandle;
};
