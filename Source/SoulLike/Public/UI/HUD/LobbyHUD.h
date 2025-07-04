// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class USoulLikeUserWidget;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USoulLikeUserWidget> LobbyMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoulLikeUserWidget> LobbyMenu;

protected:

	void BeginPlay() override;
};
