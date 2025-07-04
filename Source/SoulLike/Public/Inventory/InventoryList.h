#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryList.generated.h"

/**
 * Fast Array Replication 시스템을 사용해서 인벤토리를 구현한다.
 */

class UInventoryItemInstance;
class UEquipmentItemInstance;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryItemInstance> ItemInstance;

	bool operator<(const FInventoryListItem& other) const;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>( Items, DeltaParams, *this );
	}
	
	void AddItem(UInventoryItemInstance* InItemInstance);
	void RemoveItem(UInventoryItemInstance* InItemInstance);
	void RemoveAll();

	void SortItems();
	
	TArray<FInventoryListItem>& GetItemsRef() { return Items; }

protected:
	
	UPROPERTY()
	TArray<FInventoryListItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};

USTRUCT(BlueprintType)
struct FEquipmentInventoryList : public FInventoryList
{
	GENERATED_BODY()

	void Init(int32 InMaxIndex);
	void Register(UInventoryItemInstance * InItemInstance, int32 Index);
	void UnRegister(int32 Index);

	UEquipmentItemInstance* GetEquipmentItemInstance(int32 Index);

	int32 GetMaxIndex() const { return MaxIndex; }
		
protected:

	UPROPERTY()	
	int32 MaxIndex = 3;
};