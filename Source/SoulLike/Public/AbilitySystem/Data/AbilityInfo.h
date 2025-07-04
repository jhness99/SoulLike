// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Interface/InteractionInterface.h"
#include "SoulLikeItemTypes.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FSoulLikeAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminaCost = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

USTRUCT(BlueprintType)
struct FEnemyAbilityDamageInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponDamageInfo> DamageInfos;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	FSoulLikeAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	FInteractionTaskInfo FindInteractionTaskInfoForTag(const FGameplayTag& InteractionTag, bool bLogNotFound = false) const;

	bool ChangeAbilityInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	FGameplayTag GetAbilityInputTag(const FGameplayTag& Ability);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FSoulLikeAbilityInfo> AbilityInformation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyAbilityInfo")
	TObjectPtr<UDataTable> EnemyAbilityDamageInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InteractionAbilityInfomation")
	TArray<FInteractionTaskInfo> InteractionTaskInfomation;
};
