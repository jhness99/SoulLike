// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Inventory/InventoryItemInstance.h"
#include "RegisterableItemInstance.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class SOULLIKE_API URegisterableItemInstance : public UInventoryItemInstance
{
	GENERATED_BODY()
	
public:

	void UsingItem();
	
	UAnimMontage* GetUsingMontage() const;
	
	virtual void SetRegisted(bool InbRegisted) override { RegistInfo.bRegisted = InbRegisted; }
	virtual bool GetRegisted() const override { return RegistInfo.bRegisted; }

	void SetAppliedActiveGEHandle(const FActiveGameplayEffectHandle& InAppliedActiveGEHandle) { AppliedActiveGEHandle = InAppliedActiveGEHandle; }
	const FActiveGameplayEffectHandle& GetAppliedActiveGEHandle() const { return AppliedActiveGEHandle; }

	virtual void SetRegistInfo(const FRegistInfo& InRegistInfo) override { RegistInfo = InRegistInfo; }
	virtual FRegistInfo GetRegistInfo() const override { return RegistInfo; }
	
private:

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FRegistInfo RegistInfo;

	UPROPERTY()
	FActiveGameplayEffectHandle AppliedActiveGEHandle;
};
