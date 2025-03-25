// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoulLikeGameInstance.generated.h"

class UItemDataAsset;
class UAbilityInfo;
class UCharacterDataAsset;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<UAbilityInfo> AbilityInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;
};
