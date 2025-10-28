// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "Inventory/InventoryComponent.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangedVisibleSignature, bool, bVisible, float, Duration);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOverlappedInteractionActor, const FGameplayTag&, InteractionTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOverlappedInteractionActor, AActor*, InteractionActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEncounteredBoss, AActor*, EnemyActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickedUpItem, UItemData*, ItemData);

/**
 *  
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UOverlayWidgetController : public USoulLikeWidgetController
{
	GENERATED_BODY()

public:
	
	void BindToInventoryComponent(UInventoryComponent* InInventoryComponent);

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToModels() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRegistedItemSignature OnRegistedItemToWidget;

	UPROPERTY(BlueprintAssignable)
	FOnBeginOverlappedInteractionActor OnBeginOverlappedInteractionActorDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEncounteredBoss OnEncounteredBossDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPickedUpItem OnPickedUpItemDelegate;
	
protected:

	UFUNCTION()
	void RefreshRegistSlotAtOverlay(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag, int32 Index);

	UFUNCTION()
	void OnPickedUpItem(UItemData* ItemData);
	
// private:
//
// 	UPROPERTY()
// 	TObjectPtr<UInventoryComponent> InventoryComponent;
};
