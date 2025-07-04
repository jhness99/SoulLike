// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SL_InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FSL_InputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

USTRUCT(BlueprintType)
struct FAbilityKeyBind
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityTag = FGameplayTag();
 
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * InputAction과 GameplayTag를 매칭해서 저장하는 DataAsset
 */
UCLASS()
class SOULLIKE_API USL_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;
	FGameplayTag FindInputTagForAbilityTags(const FGameplayTagContainer& AbilityTags) const;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSL_InputAction> InputActions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FAbilityKeyBind> AbilityKeyBinds;
};
