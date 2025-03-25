// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"

#include "Engine/ActorChannel.h"
#include "Player/SoulLikePlayerState.h"

#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags){

	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;
		if(IsValid(ItemInstance)){
			WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return WroteSomething;
}

void UInventoryComponent::NextSlot(EWeaponSlot WeaponSlot)
{
	FEquipmentInventoryList EquipmentSlot;
	switch(WeaponSlot)
	{
	case EWeaponSlot::EWS_Left:
		{
		
		}
	case EWeaponSlot::EWS_Right:
		{
			EquipmentSlot = RightWeaponList;
		}
	default:
		break;
	}
	int32 MaxIndex = EquipmentSlot.GetMaxIndex();
	if(CurrentWeapon) CurrentWeapon->OnUnEquip();
	WeaponSlotIndex = WeaponSlotIndex == MaxIndex-1 ? 0 : WeaponSlotIndex + 1;
	CurrentWeapon = RightWeaponList.GetItemsRef()[WeaponSlotIndex].ItemInstance;
	if(CurrentWeapon) CurrentWeapon->OnEquip(GetOwner());
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::Init(UItemDataAsset* ItemDataAsset)
{
	RightWeaponList.Init();
	int32 Index = 0;
	for(const FInventoryData& Data : DefaultWeapons)
	{
		UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>(GetOwner());
		ItemInstance->Init(Data);
		
		RightWeaponList.Register(ItemInstance, Index++);
		InventoryList.AddItem(ItemInstance);
	}
	
	// for(const FSL_EquipmentData& Data : DefaultInventoryItems)
	// {
	// 	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
	// 	ItemInstance->Init(Data);
	// 	
	// 	InventoryList.AddItem(ItemInstance);
	// }
}

void UInventoryComponent::EquipItem()
{
	CurrentWeapon = RightWeaponList.GetItemsRef()[WeaponSlotIndex].ItemInstance;
	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnEquip(GetOwner());
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, WeaponSlotIndex);
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, RightWeaponList);
	DOREPLIFETIME(UInventoryComponent, RegisterableList);
	DOREPLIFETIME(UInventoryComponent, CurrentWeapon);
}
