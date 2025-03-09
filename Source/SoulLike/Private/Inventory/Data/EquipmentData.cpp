// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Data/EquipmentData.h"

FSL_EquipmentData UEquipmentData::FindEquipmentDataFromItemId(int32 InItemID, EEquipmentType EquipmentType) const
{
	TArray<FSL_EquipmentData> EquipmentData;

	switch(EquipmentType)
	{
	case EEquipmentType::EET_Gear:
		for(const FSL_EquipmentData& Data : GearData)
		{
			if(Data.ItemID == InItemID)
			{
				return Data;
			}
		}
	case EEquipmentType::EET_Weapon:
		
		for(const FSL_EquipmentData& Data : WeaponData)
		{
			if(Data.ItemID == InItemID)
			{
				return Data;
			}
		}
	default:
		
		return FSL_EquipmentData();
	}
}
