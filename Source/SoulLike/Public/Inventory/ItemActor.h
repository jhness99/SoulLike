// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class UItemData;
class UInventoryItemInstance;
struct FSL_ItemData;

/**
 * 장비 아이템을 레벨에 구현하는 액터
 */
UCLASS()
class SOULLIKE_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AItemActor();
	
	//void Init(UItemData* Data);
	void Init(UInventoryItemInstance* InItemInstance);
	void SetCollisionEnable(bool bEnable) const;

protected:
	/**
	 * UObject는 Replicate가 자체적으로 안되므로 ReplicateSubobjects로 직접 해야함
	 */
	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	virtual void BeginPlay() override;
	virtual void InitInternal();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance);
	
	UPROPERTY(ReplicatedUsing = OnRep_ItemInstance)
	TObjectPtr<UInventoryItemInstance> ItemInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY()
	TObjectPtr<UItemData> ItemData;
};
