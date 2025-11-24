#include "Inventory/InventoryList.h"

#include "Inventory/EquipmentItemInstance.h"


bool FInventoryListItem::operator<(const FInventoryListItem& other) const
{
	if (!ItemInstance || !other.ItemInstance)
	{
		return ItemInstance != nullptr;
	}
	
	if(ItemInstance->GetItemType() < other.ItemInstance->GetItemType())
	{
		return true;
	}
	if(ItemInstance->GetItemType() == other.ItemInstance->GetItemType())
	{
		return ItemInstance->GetItemId() < other.ItemInstance->GetItemId();
	}
	return false;
}

void FInventoryList::AddItem(UInventoryItemInstance* InItemInstance)
{
	if (!IsValid(InItemInstance))
	{
		//UE_LOG(LogTemp, Display, TEXT("InItemInstance not valid"));
		return;
	}
	
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	if (!IsValid(InItemInstance))
	{
		return;
	}
	
	for(auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if(Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			MarkItemDirty(Item);
			ItemIter.RemoveCurrent();
			Item.ItemInstance = nullptr;
			//MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveAll()
{
	// for(auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	// {
	// 	FInventoryListItem Item = *ItemIter;
	// 	ItemIter.RemoveCurrent();
	// 	MarkArrayDirty();
	// }
	Items.Empty();
	MarkArrayDirty();
}

void FInventoryList::SortItems()
{
	Items.Sort();
	MarkArrayDirty();
}

void FEquipmentInventoryList::Init(int32 InMaxIndex)
{
	MaxIndex = InMaxIndex;
	Items.Empty();
	Items.SetNum(MaxIndex);
	MarkArrayDirty();
}

void FEquipmentInventoryList::Register(UInventoryItemInstance* InItemInstance, int32 Index)
{
	// if(Items[Index].ItemInstance != nullptr)
	// {
	// 	Items[Index].ItemInstance->SetRegisted(false);
	// }
	//
	// Items[Index].ItemInstance = InItemInstance;
	// Items[Index].ItemInstance->SetRegisted(true);
	// MarkItemDirty(Items[Index]);
	if (!Items.IsValidIndex(Index))
	{
		return;
	}
	if (IsValid(Items[Index].ItemInstance))
	{
		Items[Index].ItemInstance->SetRegisted(false);
	}
	Items[Index].ItemInstance = InItemInstance;

	if (IsValid(Items[Index].ItemInstance))
	{
		Items[Index].ItemInstance->SetRegisted(true);
	}
    
	MarkItemDirty(Items[Index]);
}

void FEquipmentInventoryList::UnRegister(int32 Index)
{
	// if(Items[Index].ItemInstance == nullptr) return;
	// Items[Index].ItemInstance->SetRegisted(false);
	// Items[Index].ItemInstance = nullptr;
	// MarkArrayDirty();
	if (!Items.IsValidIndex(Index) || !IsValid(Items[Index].ItemInstance))
	{
		return;
	}
    
	Items[Index].ItemInstance->SetRegisted(false);
	Items[Index].ItemInstance = nullptr;
    
	// [수정 7] 전체 배열(MarkArrayDirty) 대신 변경된 아이템만 업데이트
	MarkItemDirty(Items[Index]);
}

UEquipmentItemInstance* FEquipmentInventoryList::GetEquipmentItemInstance(int32 Index)
{
	// if(GetMaxIndex() <= Index) return nullptr;
	//
	// return Cast<UEquipmentItemInstance>(GetItemsRef()[Index].ItemInstance);
	if (!Items.IsValidIndex(Index))
	{
		return nullptr;
	}
    
	return Cast<UEquipmentItemInstance>(Items[Index].ItemInstance);
}
