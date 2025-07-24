#include "Inventory/InventoryList.h"

#include "Inventory/EquipmentItemInstance.h"


bool FInventoryListItem::operator<(const FInventoryListItem& other) const
{
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
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	for(auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem Item = *ItemIter;
		if(Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveAll()
{
	for(auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem Item = *ItemIter;
		ItemIter.RemoveCurrent();
		MarkArrayDirty();
	}
}

void FInventoryList::SortItems()
{
	Items.Sort();
}

void FEquipmentInventoryList::Init(int32 InMaxIndex)
{
	MaxIndex = InMaxIndex;
	for(int i = 0; i < MaxIndex; i++)
	{
		FInventoryListItem& Item = Items.AddDefaulted_GetRef();
		MarkItemDirty(Item);
	}
}

void FEquipmentInventoryList::Register(UInventoryItemInstance* InItemInstance, int32 Index)
{
	if(Items[Index].ItemInstance != nullptr)
	{
		Items[Index].ItemInstance->SetRegisted(false);
	}
	
	Items[Index].ItemInstance = InItemInstance;
	Items[Index].ItemInstance->SetRegisted(true);
	MarkItemDirty(Items[Index]);
}

void FEquipmentInventoryList::UnRegister(int32 Index)
{
	if(Items[Index].ItemInstance == nullptr) return;
	Items[Index].ItemInstance->SetRegisted(false);
	Items[Index].ItemInstance = nullptr;
	MarkArrayDirty();
}

UEquipmentItemInstance* FEquipmentInventoryList::GetEquipmentItemInstance(int32 Index)
{
	if(GetMaxIndex() <= Index) return nullptr;
	
	return Cast<UEquipmentItemInstance>(GetItemsRef()[Index].ItemInstance);
}
