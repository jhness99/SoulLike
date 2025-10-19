// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "SoulLikeItemTypes.h"
#include "Game/SoulLikeSaveGame.h"
#include "Interface/SaveInterface.h"
#include "InventoryComponent.generated.h"


class UItemDataAsset;
class AItemActor;
class UInventoryItemInstance;
class UEquipmentItemInstance;
struct FSL_EquipmentData;
struct FSL_WeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRegistedItemSignature, URegisterableItemInstance*, ItemInstance, const FGameplayTag&, SlotTag, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickedUpItemSignature, UItemData*, ItemData);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULLIKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	void BindToWidgetController();

	void Init();
	void SetupDefaultInventoryList();
	void Reset();
	void EquipItem();
	void AddItemToInventoryList(FInventoryData InventoryData);
	
	UFUNCTION()
	void UpgradeItem(URegisterableItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void NextSlot(const FGameplayTag& SlotTag);
	void UpdateInventoryListToWidgetController();

	void UsingTool(URegisterableItemInstance*& ItemInstance);
	void RefillPotion();

	UFUNCTION(BlueprintCallable)
	bool CheckRequirement(URegisterableItemInstance* ItemInstance) const;
	
	UEquipmentItemInstance* GetCurrentWeapon() const { return CurrentRightWeapon; }
	UEquipmentItemInstance* GetCurrentTool() const { return CurrentTool; }
	int32 GetItemNumWithTag(const FGameplayTag& Tag);
	UInventoryItemInstance* GetItemInstanceWithTag(const FGameplayTag& Tag);

	void SetDefaultWeapons(const TArray<FInventoryData>& InDefaultWeapons);

	int32 GetRightWeaponSlotIndex() const { return RightWeaponSlotIndex; }
	int32 GetLeftWeaponSlotIndex() const { return LeftWeaponSlotIndex; }
	int32 GetToolSlotIndex() const { return ToolSlotIndex; }
	
	void SetRightWeaponSlotIndex(int32 SlotIndex) { RightWeaponSlotIndex = SlotIndex; }
	void SetLeftWeaponSlotIndex(int32 SlotIndex) { LeftWeaponSlotIndex = SlotIndex; }
	void SetToolSlotIndex(int32 SlotIndex) { ToolSlotIndex = SlotIndex; }

	TArray<FSavedItem> GetSavedItemFromInventoryList();
	void LoadInventoryListFromSavedItems(const TArray<FSavedItem>& SavedItems);
	
	// FOnRegistedItemSignature OnRegistedItem;
	FOnRegistedItemSignature RefreshRegistSlotAtOverlay;
	FOnPickedUpItemSignature PickedUpDelegate;

	//Save Interface
	FMarkAsDirtyDelegate MarkAsDirtyDelegate;

protected:
	
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
	UFUNCTION(Server, Reliable)
	void RegistItem(URegisterableItemInstance* ItemInstance, const FGameplayTag& SlotTag, int32 Index);
	
	UFUNCTION(Server, Reliable)
	void UnRegistItem(URegisterableItemInstance* ItemInstance);

private:

	FEquipmentInventoryList& FindEquipmentSlotWithTag(const FGameplayTag& SlotTag);
	void RemoveConsumeItem(UInventoryItemInstance* ItemInstance);
	void ApplyPassiveEffectRegisted(URegisterableItemInstance* ItemInstance) const;
	void RemovePassiveEffectRegisted(URegisterableItemInstance* ItemInstance) const;
	
	void RefreshCurrentSlot(FEquipmentInventoryList EquipmentSlot, const FGameplayTag& SlotTag);

	int32 GetSlotIndex(const FGameplayTag& SlotTag) const;

	void AddItem(UInventoryItemInstance* InItemInstance);
	void RemoveItem(UInventoryItemInstance* InItemInstance);

	UPROPERTY(Replicated)
	TObjectPtr<UEquipmentItemInstance> CurrentRightWeapon;

	UPROPERTY(Replicated)
	TObjectPtr<UEquipmentItemInstance> CurrentLeftWeapon;

	UPROPERTY(Replicated)
	TObjectPtr<UEquipmentItemInstance> CurrentTool;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryData> DefaultWeapons;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryData> DefaultInventoryItems;

	UPROPERTY(Replicated)
	int32 RightWeaponSlotIndex = 0;

	UPROPERTY(Replicated)
	int32 LeftWeaponSlotIndex = 0;

	UPROPERTY(Replicated)
	int32 ToolSlotIndex = 0;
	
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList RightWeaponList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList LeftWeaponList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList AccessoryList;

	UPROPERTY(Replicated)
	FEquipmentInventoryList ToolList;

	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemInstance>> OwnedItemInstances;
	
};
