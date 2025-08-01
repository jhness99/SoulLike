// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "SoulLikePlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class USoulLikeAttributeSet;
class UInventoryComponent;
class UEquipmentData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32);

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ASoulLikePlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	int32 GetPlayerLevel() const { return PlayerLevel; }
	int32 GetExp() const { return Exp; }
	int32 GetMaxPotion() const { return MaxPotion; }

	void SetPlayerLevel(int32 InLevel);
	void SetExp(int32 InExp);
	void SetMaxPotion(int32 InMaxPotion);
	
	void AddToPlayerLevel(int32 InLevel);
	void AddToExp(int32 InExp);
	void AddToMaxPotion(int32 InMaxPotion);

	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnExpChangedDelegate;
	FOnPlayerStatChanged OnMaxPotionChangedDelegate;

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> AttributeEffectClass;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PlayerLevel)
	int32 PlayerLevel = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Exp)
	int32 Exp = 1000000;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_MaxPotionNum)
	int32 MaxPotion = 3;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldPlayerLevel);

	UFUNCTION()
	void OnRep_Exp(int32 OldExp);
	
	UFUNCTION()
	void OnRep_MaxPotionNum(int32 OldMaxPotionNum);
};
