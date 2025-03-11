// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Data/ItemDataAsset.h"

UItemData* UItemDataAsset::FindItemDataFromIndexAndItemType(EItemType ItemType, FName ItemID) const
{
	switch(ItemType)
	{
	case EItemType::EIT_Weapon:
		{
			FSL_WeaponData* WeaponData = WeaponDataTable->FindRow<FSL_WeaponData>(ItemID, FString("Not Found"));
			if(WeaponData == nullptr) return nullptr;
		
			UWeaponData* WeaponDataClass = NewObject<UWeaponData>();
			WeaponDataClass->Init(*WeaponData);
		
			return WeaponDataClass;
		}
		
	case EItemType::EIT_Registerable:
		{
			FSL_RegisterableItemData* RegisterableItemData = RegisterableItemDataTable->FindRow<FSL_RegisterableItemData>(ItemID, FString("Not Found"));
			if(RegisterableItemData == nullptr) return nullptr;
		
			URegisterableItemData* RegisterableItemDataClass = NewObject<URegisterableItemData>();
			RegisterableItemDataClass->Init(*RegisterableItemData);
		
			return RegisterableItemDataClass;
		}
	}
	return nullptr;
}
