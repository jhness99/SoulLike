// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolingInterface.generated.h"

struct FEnemyData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisabledObjectSignature, AActor*, Object);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULLIKE_API IObjectPoolingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Init(AActor* InSpanwerActor, FEnemyData Data) = 0;
	virtual void Disable(bool bKilled = false) = 0;
		
	FOnDisabledObjectSignature OnDisabledObjectDelegate;
};
