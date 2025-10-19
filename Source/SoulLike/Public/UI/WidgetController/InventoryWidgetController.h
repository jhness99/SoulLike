// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "Inventory/InventoryList.h"
#include "Inventory/InventoryComponent.h"
#include "GameplayTagContainer.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedInventorySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradedItemSignature, URegisterableItemInstance*, ItemInstance);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UInventoryWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:

	void BindToInventoryComponent(UInventoryComponent* InInventoryComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BroadcastOnRegistedItem(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BroadcastUpgrade(URegisterableItemInstance* ItemInstance);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetItemNumWithTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckRequirementItem(URegisterableItemInstance* ItemInstance);
	
	UPROPERTY(BlueprintAssignable)
	FOnChangedInventorySignature OnChangedInventory;

	UPROPERTY(BlueprintAssignable)
	FOnRegistedItemSignature OnRegistedItemToWidget;

	UPROPERTY(BlueprintAssignable)
	FOnRegistedItemSignature OnRegistedItemToInventoryComponent;
	
	FOnRegistedItemSignature OnRegistedItem;

	FOnUpgradedItemSignature OnUpgradedItem;

	UPROPERTY(BlueprintReadWrite)
	int32 SelectedSlotIndex;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag SelectedSlotTag;

	UPROPERTY(BlueprintReadOnly)
	FInventoryList InventoryList;

protected:

	UFUNCTION()
	void OnRegistedItemFunc(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag, int32 Index);
	
private:

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
};
