// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AutoSaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UAutoSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Init();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AutoSaveSubsystem")
	float AutoSaveFrequency = 60.f;

private:

	void HandleAutoSave();

	FTimerHandle AutoSaveTimer;
};
