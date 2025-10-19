// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Data/ItemDataAsset.h"

#include "Game/SoulLikeGameInstance.h"

#include "SoulLikeItemTypes.h"
#include "SoulLikeGameplayTags.h"

#include "Net/UnrealNetwork.h"

UInventoryItemInstance::UInventoryItemInstance()
{
	
}

void UInventoryItemInstance::Init(const FInventoryData& Data)
{
	InventoryData = Data;
	SetupItemData(GetOuter());
}

int32 UInventoryItemInstance::GetItemType() const
{
	if(GetWeaponData())
	{
		const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
		return *GameplayTags.WeaponTypeIndex.Find(GetWeaponData()->ItemType);
	}
	return 0;
}

int32 UInventoryItemInstance::GetItemId() const
{
	if(GetItemData())
	{
		return FCString::Atoi(*GetItemData()->ItemID.ToString());
	}
	return 0;
}

void UInventoryItemInstance::SetItemNum(int32 InCount)
{
	InventoryData.Count = InCount;
}

int32 UInventoryItemInstance::GetItemNum() const
{
	return InventoryData.Count;
}

void UInventoryItemInstance::SetUpgradeLevel(int32 InUpgradeLevel)
{
	InventoryData.UpgradeLevel = InUpgradeLevel;
}

int32 UInventoryItemInstance::GetUpgradeLevel() const
{
	return InventoryData.UpgradeLevel;
}

void UInventoryItemInstance::SetInventoryData(FInventoryData InInventoryData)
{
	InventoryData = InInventoryData;
}

FInventoryData UInventoryItemInstance::GetInventoryData() const
{
	return InventoryData;
}


void UInventoryItemInstance::SetupItemData(UObject* Outer)
{
	if(GetWorld() && ItemData == nullptr)
	{
		if(USoulLikeGameInstance* GameInstance = Cast<USoulLikeGameInstance>(GetWorld()->GetGameInstance()))
		{
			ItemData = GameInstance->ItemDataAsset->FindItemDataFromIndexAndItemType(Outer, InventoryData.ItemType, InventoryData.ItemID);
		}
	}
}

void UInventoryItemInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UInventoryItemInstance::OnRep_InventoryData(FInventoryData OldInventoryData)
{
	SetupItemData(GetOuter());
}


void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryItemInstance, InventoryData);
}
