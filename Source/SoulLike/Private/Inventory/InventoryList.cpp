#include "Inventory/InventoryList.h"

#include "Inventory/InventoryItemInstance.h"


void FInventoryList::AddItem(UInventoryItemInstance* InItemInstance)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	for(auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter){
		FInventoryListItem Item = *ItemIter;
		if(Item.ItemInstance && Item.ItemInstance == InItemInstance){
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FEquipmentInventoryList::Init()
{
	for(int i = 0; i < MaxIndex; i++){

		FInventoryListItem& Item = Items.AddDefaulted_GetRef();
		MarkItemDirty(Item);
	}
}

void FEquipmentInventoryList::Register(UInventoryItemInstance* InItemInstance, int32 Index)
{
	if(InItemInstance == nullptr) {
		Items[Index].ItemInstance = nullptr;
		return;
	}
	Items[Index].ItemInstance = InItemInstance;
}

void FEquipmentInventoryList::UnRegister(int32 Index)
{
	if(Items[Index].ItemInstance == nullptr) return;
	Items[Index].ItemInstance = nullptr;
}
