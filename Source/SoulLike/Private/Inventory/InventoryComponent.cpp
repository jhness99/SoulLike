// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemActor.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Data/EquipmentData.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::Init(const UEquipmentData* EquipmentData)
{
	RightWeaponList.Init();
	int32 Index = 0;
	for(const FSL_EquipmentData& Data : DefaultWeapons)
	{
		UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
		ItemInstance->Init(Data);
		RightWeaponList.Register(ItemInstance, Index++);
		
		InventoryList.AddItem(ItemInstance);
	}
	
	for(const FSL_EquipmentData& Data : DefaultInventoryItems)
	{
		UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
		ItemInstance->Init(Data);
		
		InventoryList.AddItem(ItemInstance);
	}
}

void UInventoryComponent::EquipItem()
{
	CurrentWeapon = RightWeaponList.GetItemsRef()[0].ItemInstance;
	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnEquip(GetOwner());
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, RightWeaponList);
}
