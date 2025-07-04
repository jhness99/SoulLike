// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category  = "Roll Animation")
    UAnimMontage* HeavyRoll;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category  = "Roll Animation")
    UAnimMontage* MiddleRoll;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category  = "Roll Animation")
    UAnimMontage* LightRoll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "React Properties")
	TSoftObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "React Properties")
	TSoftObjectPtr<UAnimMontage> RiposteReactMontage;
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

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttirbutes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttirbutes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalRegeneration;
};
