// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SoulLikeAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static USoulLikeAssetManager* Get();

protected:

	virtual void StartInitialLoading() override;
};
