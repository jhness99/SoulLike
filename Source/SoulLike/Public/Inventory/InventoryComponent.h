// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "SoulLikeItemTypes.h"
#include "InventoryComponent.generated.h"


class UItemDataAsset;
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

	void Init(UItemDataAsset* ItemDataAsset);
	void EquipItem();

	UInventoryItemInstance* GetCurrentWeapon() const {return CurrentWeapon; }

protected:

	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

private:

	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryData> DefaultWeapons;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryData> DefaultInventoryItems;
	
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList RightWeaponList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList RegisterableList;
};
