// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoulLikeItemTypes.h"
#include "CombatInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULLIKE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void EquipOnCharacter(AActor* Equipment);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UInventoryItemInstance* GetCurrentWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UItemData* GetCurrentWeaponItemData();

	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* GetCurrentWeaponMontage();

	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* EvaluateRollingMontage();

	UFUNCTION(BlueprintNativeEvent)
	void NextSlot(EWeaponSlot WeaponSlot);

	UFUNCTION(BlueprintNativeEvent)
	void MeleeTrace(const FVector& TraceStart, const FVector& TraceEnd, float Radius);

	UFUNCTION(BlueprintNativeEvent)
	void ClearIgnoreActors();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWarpingLocationAndRotation();

	UFUNCTION(BlueprintNativeEvent)
	void SetWarpingLocation();

	UFUNCTION(BlueprintNativeEvent)
	void SetWarpingRotation();
};
