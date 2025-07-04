// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/InteractionInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inventory/InventoryList.h"
#include "SoulLikeFunctionLibrary.generated.h"

class USoulLikeGameInstance;
struct FSoulLikeAttributeInfo;
struct FSoulLikeDamageTypeInfo;
struct FWidgetControllerParams;
struct FSoulLikeAbilityInfo;
struct FTraceProperties;
class UDamageTypeInfo;
class UUISubsystem;
class ASoulLikeHUD;
class UAttributeSet;
class UOverlayWidgetController;
class UMenuWidgetController;
class UInventoryWidgetController;
class UAttributeSetWidgetController;
class ULevelUpMenuWidgetController;
class UConfirmMenuWidgetController;
class UUpgradeMenuWidgetController;
class UKeybindMenuWidgetController;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Widget Controller
	 */
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, ASoulLikeHUD*& OutSoulLikeHUD, FWidgetControllerParams& OutWCParams);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UAttributeSetWidgetController* GetAttributeSetWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static ULevelUpMenuWidgetController* GetLevelUpMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UConfirmMenuWidgetController* GetConfirmMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UUpgradeMenuWidgetController* GetUpgradeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|WidgetController")
	static UKeybindMenuWidgetController* GetKeybindMenuWidgetController(const UObject* WorldContextObject);
	
	/**
	 * Game Instance
	 */
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static USoulLikeGameInstance* GetSL_GameInstance(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static UUISubSystem* GetUISubSystem(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static UDamageTypeInfo* GetDamageTypeInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static UAttributeInfo* GetAttributeInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static FSoulLikeAbilityInfo GetAbilityInfoForTag(const UObject* WorldContextObject, const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static FSoulLikeDamageTypeInfo GetDamageTypeInfoForTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static FSoulLikeAttributeInfo GetAttributeInfoForTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static const FInteractionTaskInfo GetInteractionTaskInfoForTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static float GetBoostedDamageFromAttribute(const UObject* WorldContextObject, UAttributeSet* AS, const FGameplayTag& DamageTag, float Damage);
	
	/**
	 * Item Data
	 */

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|ItemData")
	static FString GetTagNameFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|ItemData")
	static FString GetItemTypeFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|ItemData")
	static FString GetWeaponTypeFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|ItemData")
	static FString GetToolTypeFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|ItemData")
	static TArray<FInventoryListItem>& GetInvenoryListItems(FInventoryList InventoryList);

	/**
	 * Gameplay Mechanics
	 */

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static AActor* ClosestActorWithHitResults(const UObject* WorldContextObject, const TArray<FHitResult>& HitResults);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static bool CheckTargetOnViewport(const AActor* TargetActor, float Tolerance);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static bool CheckDistanceToTarget(const AActor* Instigator, const AActor* TargetActor, float Distance);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static bool SetupTraceProperties(const AActor* Instigator, FVector& TraceStartLocation, FVector& TraceEndLocation, FVector& TraceForwardVector, float TraceLength);
	
	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static bool SingleTraceFromCameraLocation(const AActor* Instigator, FHitResult& HitResult, FTraceProperties TraceProperties, bool bDebug = false);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameplayMechanics")
	static bool MultiTraceFromCameraLocation(const AActor* Instigator, TArray<FHitResult>& HitResults, FTraceProperties TraceProperties, bool bDebug = false);
	
	// /**
	//  * GAS
	//  */
	// UFUNCTION(BlueprintCallable, Category = "SoulLikeFunctionLibrary|GAS")
	// static bool SetAIStatusWithTag(const AActor* WorldContextActor, const FGameplayTag& StatusTag);
	//
	// /**
	//  * Effect Context Setters
	//  */
	// UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	// static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	//
	// UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	// static void SetAttackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InAttackForce);
	//
	// UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	// static void SetToughness(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InToughness);
	//
	// UFUNCTION(BlueprintCallable, Category="SoulLikeFunctionLibrary|GameplayEffects")
	// static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackForce);
	//
	// /**
	//  * Gameplay 
	//  */
	// UFUNCTION(BlueprintCallable, Category = "SoulLikeFunctionLibrary|DamageEffect")
	// static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
