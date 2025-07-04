// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Inventory/InventoryComponent.h"

void UInventoryWidgetController::BindToInventoryComponent(UInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;

	OnRegistedItemToInventoryComponent.AddDynamic(this, &UInventoryWidgetController::OnRegistedItemFunc);
}

void UInventoryWidgetController::BroadcastOnRegistedItem(URegisterableItemInstance* ItemInstance,
	const FGameplayTag& SlotTag, int32 Index)
{
	OnRegistedItemToInventoryComponent.Broadcast(ItemInstance, SlotTag, Index);
}

void UInventoryWidgetController::BroadcastUpgrade(URegisterableItemInstance* ItemInstance)
{
	OnUpgradedItem.Broadcast(ItemInstance);
}

int32 UInventoryWidgetController::GetItemNumWithTag(FGameplayTag Tag)
{
	if(InventoryComponent.IsValid())
	{
		return InventoryComponent->GetItemNumWithTag(Tag);
	}
	return 0;
}

bool UInventoryWidgetController::CheckRequirementItem(URegisterableItemInstance* ItemInstance)
{
	return InventoryComponent->CheckRequirement(ItemInstance);
}

void UInventoryWidgetController::OnRegistedItemFunc(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag,
                                                    int32 Index)
{
	OnRegistedItem.Broadcast(ItemInstance, SlotTag, Index);
	OnRegistedItemToWidget.Broadcast(ItemInstance, SlotTag, Index);
}
