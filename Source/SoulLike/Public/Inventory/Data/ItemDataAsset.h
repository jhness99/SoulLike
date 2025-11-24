// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SoulLikeItemTypes.h"
#include "ItemDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FItemDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTypeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemData> ItemDataClass;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//UItemData* FindItemDataFromIndexAndItemType(FGameplayTag ItemType, FName ItemID) const;
	UItemData* FindItemDataFromIndexAndItemType(UObject* Outer, FGameplayTag ItemType, FName ItemID);

	FString FindTagNameFromGameplayTag(const FGameplayTag& Tag) const;
	FString FindItemTypeFromGameplayTag(const FGameplayTag& Tag) const;
	FString FindWeaponTypeFromGameplayTag(const FGameplayTag& Tag) const;
	FString FindToolTypeFromGameplayTag(const FGameplayTag& Tag) const;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TArray<FItemTypeMapping> ItemTypeMappings;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<UDataTable> ItemDataTable;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<UDataTable> WeaponDataTable;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<UDataTable> GearDataTable;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<UDataTable> ToolDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FString> StringToEquipSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FString> StringToItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FString> StringToWeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FString> StringToToolType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemDataTable> ItemDataTables;

	UPROPERTY()
	TMap<FName, TObjectPtr<UItemData>> ItemDataToItemID;
};
