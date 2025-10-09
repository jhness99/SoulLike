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

	UFUNCTION(BlueprintPure)
	UConfirmMenuWidgetController* GetConfirmMenuWidgetController();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USoulLikeUserWidget> LobbyMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoulLikeUserWidget> LobbyMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UConfirmMenuWidgetController> ConfirmMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UConfirmMenuWidgetController> ConfirmMenuWidgetController;

protected:

	void BeginPlay() override;
};
