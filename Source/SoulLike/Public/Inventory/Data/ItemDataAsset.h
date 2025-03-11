// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SoulLikeItemTypes.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UItemData* FindItemDataFromIndexAndItemType(EItemType ItemType, FName ItemID) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> RegisterableItemDataTable;
};
