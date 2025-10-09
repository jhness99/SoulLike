// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULLIKE_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsUsedObject() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsUsedObject(bool InIsUsedObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDirty() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MarkAsClean();
};
