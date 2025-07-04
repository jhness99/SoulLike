// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "AttributeSetWidgetController.generated.h"

struct FGameplayTag;
struct FGameplayAttribute;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UAttributeSetWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToModels() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatusChangedSignature LevelChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatusChangedSignature ExpChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
