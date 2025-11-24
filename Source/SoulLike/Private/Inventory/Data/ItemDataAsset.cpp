// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Data/ItemDataAsset.h"

#include "SoulLikeGameplayTags.h"

UItemData* UItemDataAsset::FindItemDataFromIndexAndItemType(UObject* Outer, FGameplayTag ItemType, FName ItemID)
{
	for(const FItemDataTable& ItemDataTableStruct : ItemDataTables)
	{
		if(ItemType.MatchesTagExact(ItemDataTableStruct.ItemTypeTag))
		{
			FSL_ItemData* ItemData = ItemDataTableStruct.DataTable->FindRow<FSL_ItemData>(ItemID, FString("Not Found"));
			if(ItemData == nullptr) return nullptr;

			if (Outer && ItemDataTableStruct.ItemDataClass != nullptr)
			{
				if (TObjectPtr<UItemData>* FoundItemPtr = ItemDataToItemID.Find(ItemData->ItemID))
				{
					if (IsValid(*FoundItemPtr))
					{
						return *FoundItemPtr;
					}
					ItemDataToItemID.Remove(ItemData->ItemID);
				}
				
				UItemData* ItemDataObject = NewObject<UItemData>(Outer, ItemDataTableStruct.ItemDataClass.Get());
				ItemDataObject->Init(ItemData);

				ItemDataToItemID.Add(ItemData->ItemID, ItemDataObject);

				return ItemDataObject;
			}
		
		}
	}
	
	return nullptr;
}

FString UItemDataAsset::FindTagNameFromGameplayTag(const FGameplayTag& Tag) const
{
	if(const FString* String = StringToEquipSlot.Find(Tag))
	{
		return *String;
	}
	return FString();
}

FString UItemDataAsset::FindItemTypeFromGameplayTag(const FGameplayTag& Tag) const
{
	if(const FString* String = StringToItemType.Find(Tag))
	{
		return *String;
	}
	return FString();
}


FString UItemDataAsset::FindWeaponTypeFromGameplayTag(const FGameplayTag& Tag) const
{
	if(const FString* String = StringToWeaponType.Find(Tag))
	{
		return *String;
	}
	return FString();
}

FString UItemDataAsset::FindToolTypeFromGameplayTag(const FGameplayTag& Tag) const
{
	if(const FString* String = StringToToolType.Find(Tag))
	{
		return *String;
	}
	return FString();
}
