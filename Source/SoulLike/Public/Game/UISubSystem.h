// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubSystem.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveUITaskInUIModeSignature, const FGameplayTag&, TaskTag);
DECLARE_DELEGATE(FOnCloseMenu);
/**
 * 
 */
UCLASS()
class SOULLIKE_API UUISubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	void ReceiveUITask(const FGameplayTag& TaskTag) const;
	void CloseMenu();
	
	FOnReceiveUITaskInUIModeSignature OnReceiveUITask;
	FOnCloseMenu OnCloseMainMenu;
};
