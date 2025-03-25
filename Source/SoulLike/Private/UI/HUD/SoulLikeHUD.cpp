// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SoulLikeHUD.h"

#include "UI/Widgets/SoulLikeUserWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeSetWidgetController.h"

UOverlayWidgetController* ASoulLikeHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr){
    
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToModels();
	}
	return OverlayWidgetController;
}

UAttributeSetWidgetController* ASoulLikeHUD::GetAttributeSetWidgetController(const FWidgetControllerParams& WCParams)
{
	if(AttributeSetWidgetController == nullptr){
    
		AttributeSetWidgetController = NewObject<UAttributeSetWidgetController>(this, AttributeSetWidgetControllerClass);
		AttributeSetWidgetController->SetWidgetControllerParams(WCParams);
	}
	return AttributeSetWidgetController;
}

UInventoryWidgetController* ASoulLikeHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if(InventoryWidgetController == nullptr){
    
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
	}
	return InventoryWidgetController;
}

void ASoulLikeHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitalized, Please fill out BP_SoulLikeHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitalized, Please fill out BP_SoulLikeHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USoulLikeUserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

void ASoulLikeHUD::BeginPlay()
{
	Super::BeginPlay();
}
