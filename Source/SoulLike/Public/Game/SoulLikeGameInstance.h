// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Game/SoulLikeSaveGame.h"
#include "SoulLikeGameInstance.generated.h"

class USoulLikeSaveGame;
class UEnemyDataAsset;
class UAttributeInfo;
class UItemDataAsset;
class UAbilityInfo;
class UCharacterDataAsset;
class UDamageTypeInfo;

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	virtual void Init() override;

	void SetClientSaveData(USoulLikeSaveGame* InSaveData)
	{
		UE_LOG(LogTemp, Display, TEXT("SetClientSaveData"));
		ClientSaveData = FClientSaveData(InSaveData);
	}
	const FClientSaveData& GetClientSaveData() const { return ClientSaveData; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<UItemDataAsset> ItemDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<UAbilityInfo> AbilityInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageTypeInfo")
	TObjectPtr<UDamageTypeInfo> DamageTypeInfoDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeInfo")
	TObjectPtr<UAttributeInfo> AttributeInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;
	
	//ObjectPoolingSubsystem Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyObjectPoolilngSubsystem")
	TObjectPtr<UEnemyDataAsset> EnemyDataAssset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyObjectPoolilngSubsystem")
	int32 MaxPoolSize = 100;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EnemyObjectPoolilngSubsystem")
	float MaxMobGenerateDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LoadSlotName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LoadSlotIndex = 0;

	/** AutoSaveSubsystem Properties*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AutoSaveSubsystem")
	float AutoSaveFrequency = 60.f;

	/** OnlineSessionSubsystem Properties*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnlineSessionSubsystem")
	TSubclassOf<AActor> WhiteSignActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnlineSessionSubsystem")
	float MaxSearchSignDistance = 750.f;


private:

	UPROPERTY()
	FClientSaveData ClientSaveData;
};
