// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/RegisterableItemInstance.h"
#include "EquipmentItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UEquipmentItemInstance : public URegisterableItemInstance
{
	GENERATED_BODY()
	
public:

	UAnimMontage* GetAttackMontage() const;
	UAnimMontage* GetRiposteMontage() const;
	
	void OnEquip(AActor* Owner, const FGameplayTag& SlotTag);
	void OnUnEquip();
	
	AItemActor* GetItemActor() const { return ItemActor; }

	void SetCollisionEnable(bool bEnable) const;

private:
	
	UPROPERTY(Replicated)
	TObjectPtr<AItemActor> ItemActor;
};
