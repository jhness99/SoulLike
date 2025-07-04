// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulLikeAbilityTypes.h"
#include "Character/Data/EnemyDataAsset.h"
#include "Game/SoulLikeSaveGame.h"
#include "SoulLikeAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	/**
	 * GAS
	 */
	UFUNCTION(BlueprintCallable, Category = "SoulLikeFunctionLibrary|GAS")
	static bool SetAIStatusWithTag(const AActor* WorldContextActor, const FGameplayTag& StatusTag);

	/**
	 * Ability System Class Defaults
	 */
	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, USoulLikeSaveGame* SaveData);

	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes_Enemy(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, FEnemyData EnemyData);

	/**
	 * Effect Context Setters
	 */
	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	static void SetAttackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InAttackForce);

	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	static void SetToughness(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InToughness);

	UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackForce);
	
	/**
	 * Gameplay 
	 */
	UFUNCTION(BlueprintCallable, Category = "SoulLikeFunctionLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
