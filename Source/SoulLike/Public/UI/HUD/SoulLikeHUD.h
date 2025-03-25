// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/SoulLikeWidgetController.h"
#include "SoulLikeHUD.generated.h"

class UInventoryWidgetController;
class UOverlayWidgetController;
class UMenuWidgetController;
class UAttributeSetWidgetController;
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
	UAttributeSetWidgetController* GetAttributeSetWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulLikeUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<USoulLikeUserWidget> OverlayWidget;

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
};
