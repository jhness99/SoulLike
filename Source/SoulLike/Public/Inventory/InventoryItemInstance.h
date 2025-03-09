// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Data/EquipmentData.h"
#include "InventoryItemInstance.generated.h"

class AItemActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UInventoryItemInstance();
	
	virtual bool IsSupportedForNetworking() const override { return true; } 

	void Init(const FSL_EquipmentData& Data);
	void OnEquip(AActor* Owner);
	void OnUnEquip();

	UAnimMontage* GetMontage();

protected:

	UPROPERTY(Replicated)
	FSL_EquipmentData EquipmentData;
	
	UPROPERTY(Replicated)
	TObjectPtr<AItemActor> ItemActor;
};
