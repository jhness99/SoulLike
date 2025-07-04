// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DamageTypeInfo.generated.h"

USTRUCT(BlueprintType)
struct FSoulLikeDamageTypeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DamageTypeName = FString();
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UDamageTypeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FSoulLikeDamageTypeInfo FindDamageTypeInfoWithTag(const FGameplayTag& Tag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSoulLikeDamageTypeInfo> DamageTypeInfos;
};
