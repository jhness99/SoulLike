// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AttributeSetWidgetController.h"
#include "SoulLikeAbilityTypes.h"
#include "LevelUpMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanLeveUpChangedSignature, bool, bCanLevelUp);

/**
 * 
 */
UCLASS()
class SOULLIKE_API ULevelUpMenuWidgetController : public UAttributeSetWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BLueprintCallable)
	void UpgradeAttributes(const TArray<FAttributeValue>& AttributeValues);

	UFUNCTION(BLueprintCallable)
	int32 GetTotalExp(int32 Level) const;

	UFUNCTION(BlueprintCallable)
	void AddToPlayerLevel(int32 InPlayerLevel);
	
	UFUNCTION(BlueprintCallable)
	void AddToExp(int32 InExp);

	UFUNCTION(BlueprintCallable)
	bool GetbCanLevelUp() const { return bCanLevelUp; };
	
	UFUNCTION(BlueprintCallable)
	void SetbCanLevelUp(bool InbCanLevelUp);

	UPROPERTY(BlueprintAssignable)
	FOnCanLeveUpChangedSignature ChangeCanLevelUpDelegate;

private:
	
	UPROPERTY()
	bool bCanLevelUp = false;
};
