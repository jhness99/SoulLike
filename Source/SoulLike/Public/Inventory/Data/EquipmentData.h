// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoulLikeItemTypes.h"
#include "Inventory/InventoryComponent.h"
#include "EquipmentData.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UEquipmentData : public UDataAsset
{
	GENERATED_BODY()

public:

	FSL_EquipmentData FindEquipmentDataFromItemId(int32 InItemID, EEquipmentType EquipmentType) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSL_WeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSL_GearData> GearData;
	
};

