// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoulLikeItemTypes.h"
#include "SoulLikeAbilityTypes.h"
#include "CombatInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

class URegisterableItemInstance;
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
	
	/**
	 * Inventory Func
	 */
	
	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Inventory")
	void EquipOnCharacter(AActor* Equipment, FName SocketName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UEquipmentItemInstance* GetCurrentWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UItemData* GetCurrentWeaponItemData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	void SetupCurrentWeaponItemType(const FGameplayTag& ItemType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UAnimMontage* GetCurrentWeaponAttackMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UAnimMontage* GetCurrentWeaponRiposteMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UEquipmentItemInstance* GetCurrentTool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Inventory")
	UAnimMontage* GetCurrentToolMontage();
	
	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Inventory")
	void NextSlot(const FGameplayTag& SlotTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Inventory")
	void UsingTool(URegisterableItemInstance* ItemInstance);
	/**
	 * Combat Func
	 */

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	UAnimMontage* EvaluateRollingMontage();

	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Combat")
	void MeleeTrace(const FTransform& TipStartRelativeToParent, const FTransform& TipEndRelativeToParent, float Radius);

	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Combat")
	void ApplyDamageToTarget(AActor* Target, float  DamageBoost = 1.f);

	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|Combat")
	void ClearIgnoreActors();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	void SetMovementMode(EMovementMode MovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	void SetStrafeMode(bool bStrafe);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	float GetLadderMoveInput() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	bool TryBackstab();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	void SwitchWalkSpeed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	void SwitchRunSpeed() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Combat")
	void SetWeaponCollisionEnable(bool bEnable, EMeleeTraceType MeleeTraceType, FName InAbilityIndex);
	
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	
	/**
	 * GAS Func
	 */
	
	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|GAS")
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|GAS")
	UAnimMontage* GetRiposteReactMontage();

	/**
	 * Motion Warping Func
	 */
	
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|MotionWarping")
	// void SetWarpingLocationAndRotation(FVector Location, FRotator Rotation);
	//
	// UFUNCTION(BlueprintNativeEvent, Category = "ComabatInterface|MotionWarping")
	// void SetWarpingLocation();
	//
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|MotionWarping")
	// void SetWarpingRotation(FRotator TargetRotation = FRotator::ZeroRotator);

	/**
	 * Animation Func
	 */
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Animation")
	bool GetMirror() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Animation")
	void SetMirror(bool InMirror);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Animation")
	bool GetLadderMirror() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Animation")
	void SetLadderMirror(bool InMirror);

	/**
	 * Widget Func
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComabatInterface|Widget")
	void ToggleTargetMark();
};
