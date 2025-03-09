// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "InventoryComponent.generated.h"


class UEquipmentData;
class AItemActor;
class UInventoryItemInstance;
struct FSL_EquipmentData;
struct FSL_WeaponData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULLIKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	void Init(const UEquipmentData* EquipmentData);
	void EquipItem();

	UInventoryItemInstance* GetCurrentWeapon() const {return CurrentWeapon; }

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSL_WeaponData> DefaultWeapons;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSL_EquipmentData> DefaultInventoryItems;
	
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList RightWeaponList;
};
