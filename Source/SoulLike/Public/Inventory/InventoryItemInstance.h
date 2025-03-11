// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulLikeItemTypes.h"
#include "InventoryItemInstance.generated.h"

class AItemActor;
class UItemData;
class UItemDataAsset;
struct FInventoryData;

/**
 * 인벤토리의 아이템의 정보를 저장하는 Instance
 * FInventoryData로 아이템의 정보를 받고
 * GameInstance의 ItemDataAsset에서 FInventoryData로 UItemData를 가져온다.
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOULLIKE_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UInventoryItemInstance();

	/**
	 * UObject를 Replicate하게 해준다.
	 */
	virtual bool IsSupportedForNetworking() const override { return true; }

	/**
	 * FInventoryData 를 저장. FInventoryData는 Replicated된다.
	 * @param Data 아이템의 타입과 ID
	 */
	void Init(const FInventoryData& Data);
	void OnEquip(AActor* Owner);
	void OnUnEquip();

	UAnimMontage* GetMontage() const;

protected:
	/**
	 * InventoryData를 사용해서 ItemData를 생성
	 */
	void SetupItemData();

	/**
	 * InventoryData가 서버에서 Replicate 될 때 클라이언트에서
	 * ItemDataAsset에 접근해서 해당하는 ItemData를 가져옴
	 */
	UFUNCTION()
	void OnRep_InventoryData(FInventoryData OldInventoryData);

	UPROPERTY()
	TObjectPtr<UItemData> ItemData;
	
	UPROPERTY(ReplicatedUsing = OnRep_InventoryData)
	FInventoryData InventoryData;
	
	UPROPERTY(Replicated)
	TObjectPtr<AItemActor> ItemActor;
};
