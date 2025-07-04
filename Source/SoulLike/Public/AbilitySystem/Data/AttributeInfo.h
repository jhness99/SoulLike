// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FSoulLikeAttributeInfo{

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FSoulLikeAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag) const;
	int32 GetRequireExp(int32 Level) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSoulLikeAttributeInfo> AttributeInformation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGameplayTag> DamageTypeTagToBoostAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat RequireEXP = FScalableFloat();
};
