// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulLikeFunctionLibrary.generated.h"

class ASoulLikeHUD;
class UOverlayWidgetController;
class UMenuWidgetController;
class UInventoryWidgetController;
class UAttributeSetWidgetController;
class UAbilityInfo;
struct FWidgetControllerParams;
struct FSoulLikeAbilityInfo;

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

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulLikeFunctionLibrary|GameInstance")
	static FSoulLikeAbilityInfo GetAbilityInfoForTag(const UObject* WorldContextObject, const FGameplayTag& Tag);
};
