// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* HeavyRoll;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* MiddleRoll;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* LightRoll;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPlayerData PlayerData;

	
	
};
