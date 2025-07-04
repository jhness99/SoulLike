// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "UI/WidgetController/UpgradeMenuWidgetController.h"
#include "SoulLikeHUD.generated.h"

class ULevelUpMenuWidgetController;
class UInventoryWidgetController;
class UOverlayWidgetController;
class UMenuWidgetController;
class UAttributeSetWidgetController;
class USavePointMenuWidgetController;
class UConfirmMenuWidgetController;
class UKeybindMenuWidgetController;
class USoulLikeUserWidget;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UMenuWidgetController* GetMenuWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeSetWidgetController* GetAttributeSetWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	USavePointMenuWidgetController* GetSavePointMenuWidgetController(const FWidgetControllerParams& WCParams);
	ULevelUpMenuWidgetController* GetLevelUpMenuWidgetController(const FWidgetControllerParams& WCParams);
	UConfirmMenuWidgetController* GetConfirmMenuWidgetController(const FWidgetControllerParams& WCParams);
	UUpgradeMenuWidgetController* GetUpgradeMenuWidgetController(const FWidgetControllerParams& WCParams);
	UKeybindMenuWidgetController* GetKeybindMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:

	virtual void BeginPlay() override;

private:
	
	void HandleControllMenuWidget(USoulLikeUserWidget* Widget);
	void OnPressedMainMenuButton();
	void OnOpenSavePointMenu(const FString& SavePointName);
	
	UPROPERTY()
	TObjectPtr<UUISubSystem> UISubSystem;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> CurrentWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulLikeUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulLikeUserWidget> MenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> MenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulLikeUserWidget> SavePointMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> SavePointMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulLikeUserWidget> ConfirmMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> ConfirmMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidgetController> MenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UMenuWidgetController> MenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeSetWidgetController> AttributeSetWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeSetWidgetController> AttributeSetWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USavePointMenuWidgetController> SavePointMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USavePointMenuWidgetController> SavePointMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelUpMenuWidgetController> LevelUpMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<ULevelUpMenuWidgetController> LevelUpMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UConfirmMenuWidgetController> ConfirmMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UConfirmMenuWidgetController> ConfirmMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUpgradeMenuWidgetController> UpgradeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UUpgradeMenuWidgetController> UpgradeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKeybindMenuWidgetController> KeybindMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UKeybindMenuWidgetController> KeybindMenuWidgetController;
	
};
