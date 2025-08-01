// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULLIKE_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshInventory();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerSavePointMenuWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnterBossRoom();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SaveProgress() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GiveExp(int32 ExpValue);
};
