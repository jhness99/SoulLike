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
	//OnUpgradedItem.Broadcast(ItemInstance);
	if(InventoryComponent.IsValid())
	{
		InventoryComponent->UpgradeItem(ItemInstance);
	}
}

void UInventoryWidgetController::OnRegistedItemFunc(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag,
                                                    int32 Index)
{
	OnRegistedItem.Broadcast(ItemInstance, SlotTag, Index);
	if(InventoryComponent.IsValid())
	{
		InventoryComponent->RegistItem(ItemInstance, SlotTag, Index);
	}
	OnRegistedItemToWidget.Broadcast(ItemInstance, SlotTag, Index);
}
