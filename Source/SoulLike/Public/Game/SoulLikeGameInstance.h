// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Inventory/Data/ItemDataAsset.h"
#include "SoulLikeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	TObjectPtr<UItemDataAsset> ItemDataAsset;	
};
