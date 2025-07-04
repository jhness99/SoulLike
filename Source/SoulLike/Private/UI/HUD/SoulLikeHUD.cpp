// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SoulLikeHUD.h"

#include "UI/Widgets/SoulLikeUserWidget.h"
#include "UI/WidgetController/MenuWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeSetWidgetController.h"
#include "UI/WidgetController/SavePointMenuWidgetController.h"
#include "UI/WidgetController/LevelUpMenuWidgetController.h"
#include "UI/WidgetController/ConfirmMenuWidgetController.h"
#include "UI/WidgetController/KeybindMenuWidgetController.h"

#include "Player/SoulLikePlayerController.h"

#include "Game/UISubSystem.h"

#include "SoulLikeFunctionLibrary.h"


UOverlayWidgetController* ASoulLikeHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr){
    
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToModels();
	}
	return OverlayWidgetController;
}

UMenuWidgetController* ASoulLikeHUD::GetMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(MenuWidgetController == nullptr){
    
		MenuWidgetController = NewObject<UMenuWidgetController>(this, MenuWidgetControllerClass);
		MenuWidgetController->SetWidgetControllerParams(WCParams);
		MenuWidgetController->BindCallbacksToModels();
	}
	return MenuWidgetController;
}

UAttributeSetWidgetController* ASoulLikeHUD::GetAttributeSetWidgetController(const FWidgetControllerParams& WCParams)
{
	if(AttributeSetWidgetController == nullptr){
    
		AttributeSetWidgetController = NewObject<UAttributeSetWidgetController>(this, AttributeSetWidgetControllerClass);
		AttributeSetWidgetController->SetWidgetControllerParams(WCParams);
		AttributeSetWidgetController->BindCallbacksToModels();
	}
	return AttributeSetWidgetController;
}

UInventoryWidgetController* ASoulLikeHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if(InventoryWidgetController == nullptr){
    
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToModels();
	}
	return InventoryWidgetController;
}

USavePointMenuWidgetController* ASoulLikeHUD::GetSavePointMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(SavePointMenuWidgetController == nullptr){
    
		SavePointMenuWidgetController = NewObject<USavePointMenuWidgetController>(this, SavePointMenuWidgetControllerClass);
		SavePointMenuWidgetController->SetWidgetControllerParams(WCParams);
		SavePointMenuWidgetController->BindCallbacksToModels();
	}
	return SavePointMenuWidgetController;
}

ULevelUpMenuWidgetController* ASoulLikeHUD::GetLevelUpMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(LevelUpMenuWidgetController == nullptr){
    
		LevelUpMenuWidgetController = NewObject<ULevelUpMenuWidgetController>(this, LevelUpMenuWidgetControllerClass);
		LevelUpMenuWidgetController->SetWidgetControllerParams(WCParams);
		LevelUpMenuWidgetController->BindCallbacksToModels();
	}
	return LevelUpMenuWidgetController;
}

UConfirmMenuWidgetController* ASoulLikeHUD::GetConfirmMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(ConfirmMenuWidgetController == nullptr){
    
		ConfirmMenuWidgetController = NewObject<UConfirmMenuWidgetController>(this, ConfirmMenuWidgetControllerClass);
		ConfirmMenuWidgetController->SetWidgetControllerParams(WCParams);
		ConfirmMenuWidgetController->BindCallbacksToModels();
	}
	return ConfirmMenuWidgetController;
}

UUpgradeMenuWidgetController* ASoulLikeHUD::GetUpgradeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(UpgradeMenuWidgetController == nullptr){
    
		UpgradeMenuWidgetController = NewObject<UUpgradeMenuWidgetController>(this, UpgradeMenuWidgetControllerClass);
		UpgradeMenuWidgetController->SetWidgetControllerParams(WCParams);
		UpgradeMenuWidgetController->BindCallbacksToModels();
	}
	return UpgradeMenuWidgetController;
}

UKeybindMenuWidgetController* ASoulLikeHUD::GetKeybindMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(KeybindMenuWidgetController == nullptr){
    
		KeybindMenuWidgetController = NewObject<UKeybindMenuWidgetController>(this, KeybindMenuWidgetControllerClass);
		KeybindMenuWidgetController->SetWidgetControllerParams(WCParams);
		KeybindMenuWidgetController->BindCallbacksToModels();
	}
	return KeybindMenuWidgetController;
}

void ASoulLikeHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(MenuWidgetClass, TEXT("Main Menu Widget Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(MenuWidgetControllerClass, TEXT("Main Menu Widget Controller Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(SavePointMenuWidgetClass, TEXT("Save Point Menu Widget Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(SavePointMenuWidgetControllerClass, TEXT("Save Point Menu Widget Controller Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(ConfirmMenuWidgetClass, TEXT("Confirm Menu Widget Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(ConfirmMenuWidgetControllerClass, TEXT("Confirm Menu Widget Controller Class uninitalized, Please fill out BP_SoulLikeHUD"));

	UISubSystem = USoulLikeFunctionLibrary::GetUISubSystem(this);
	
	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USoulLikeUserWidget>(Widget);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController(WidgetControllerParams));
	
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();

	Widget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
	MenuWidget = Cast<USoulLikeUserWidget>(Widget);
	MenuWidget->SetWidgetController(GetMenuWidgetController(WidgetControllerParams));

	Widget = CreateWidget<UUserWidget>(GetWorld(), SavePointMenuWidgetClass);
	SavePointMenuWidget = Cast<USoulLikeUserWidget>(Widget);
	SavePointMenuWidget->SetWidgetController(GetSavePointMenuWidgetController(WidgetControllerParams));

	Widget = CreateWidget<UUserWidget>(GetWorld(), ConfirmMenuWidgetClass);
	ConfirmMenuWidget = Cast<USoulLikeUserWidget>(Widget);
	ConfirmMenuWidget->SetWidgetController(GetConfirmMenuWidgetController(WidgetControllerParams));
	
	if(ASoulLikePlayerController* SL_PlayerController = Cast<ASoulLikePlayerController>(GetOwningPlayerController()))
	{
		SL_PlayerController->OnPressedMainMenuButton.BindUObject(this, &ASoulLikeHUD::OnPressedMainMenuButton);
		SL_PlayerController->OnTriggerSavePointMenu.BindUObject(this, &ASoulLikeHUD::OnOpenSavePointMenu);
		
	}
	if(UISubSystem)
	{
		UISubSystem->OnCloseMainMenu.BindUObject(this, &ASoulLikeHUD::OnPressedMainMenuButton);
	}
}

void ASoulLikeHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulLikeHUD::HandleControllMenuWidget(USoulLikeUserWidget* Widget)
{
	if(Widget == nullptr) return;
	
	if(ASoulLikePlayerController* SL_PlayerController = Cast<ASoulLikePlayerController>(GetOwningPlayerController()))
	{
		const bool bUIMode = SL_PlayerController->GetUIMode();
		if(!bUIMode)
		{
			Widget->AddToViewport();
		}
		else
		{
			if(!Widget->IsInViewport()) return;
			Widget->RemoveFromParent();
		}
		if(USoulLikeWidgetController* WidgetController = Cast<USoulLikeWidgetController>(Widget->WidgetController))
		{
			WidgetController->OnOpendMenu.Broadcast(!bUIMode);
		}
		OverlayWidgetController->OnOpendMenu.Broadcast(!bUIMode);
		
		if(!bUIMode)
			SL_PlayerController->SetInputModeTag(FSoulLikeGameplayTags::Get().InputMode_UI);
		else
			SL_PlayerController->SetInputModeTag(FSoulLikeGameplayTags::Get().InputMode_Game);
	}
}

void ASoulLikeHUD::OnPressedMainMenuButton()
{
	HandleControllMenuWidget(MenuWidget);
}

void ASoulLikeHUD::OnOpenSavePointMenu(const FString& SavePointName)
{
	if(SavePointMenuWidgetController)
	{
		SavePointMenuWidgetController->SetSavePointName(SavePointName);
	}
	HandleControllMenuWidget(SavePointMenuWidget);
}
