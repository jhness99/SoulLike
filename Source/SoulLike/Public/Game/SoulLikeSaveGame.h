// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "SoulLikeItemTypes.h"
#include "Inventory/RegisterableItemInstance.h"
#include "SoulLikeSaveGame.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right){

	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

USTRUCT(BlueprintType)
struct FSavedItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryData InventoryData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRegistInfo RegistInfo;
};


USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	bool bIsUsedObject = false;

	// 직렬화 된 액터의 변수
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right){

	return Left.ActorName == Right.ActorName;
}

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	FString GetPlayerName() const { return PlayerName; }
	
	UFUNCTION(BlueprintPure)
	int32 GetPlayerLevel() const { return PlayerLevel; }
	
	UPROPERTY()
	FString SlotName = FString("");
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/** Player Status */
	
	UPROPERTY()
	FString PlayerName = FString("");
	
	UPROPERTY()
	int32 PlayerLevel = 1;

	UPROPERTY()
	int32 EXP = 0;
	
	UPROPERTY()
	int32 MaxPotion = 3;

	/** Attributes */
	
	UPROPERTY()
	float Vigor = 0;

	UPROPERTY()
	float Mind = 0;

	UPROPERTY()
	float Endurance = 0;
	
	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Dexterity = 0;
	
	UPROPERTY()
	float Intelligence = 0;

	/** Abilities */
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	UPROPERTY()
	TArray<FSavedActor> SavedActors;

	/** Inventory */
	UPROPERTY()
	TArray<FSavedItem> SavedItems;
	
	UPROPERTY()
	int32 RightWeaponSlotIndex = 0;

	UPROPERTY()
	int32 LeftWeaponSlotIndex = 0;

	UPROPERTY()
	int32 ToolSlotIndex = 0;
	
	/** Location */
	UPROPERTY()
	FTransform Transform = FTransform();
};
