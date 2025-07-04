// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/UISubSystem.h"
#include "SoulLikeWidgetController.generated.h"

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;
class ASoulLikePlayerController;
class ASoulLikePlayerState;
class USoulLikeAbilitySystemComponent;
class USoulLikeAttributeSet;
class UAttributeInfo;
struct FGameplayTag;
struct FSoulLikeAttributeInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FSoulLikeAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatusChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenedMenuSignature, bool, bOpen);

/**
 * 
 */
UCLASS(BlueprintType)
class SOULLIKE_API USoulLikeWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToModels();
	
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToUISubsystem();

	UFUNCTION(BlueprintCallable)
	void BroadcastCloseMenu();
	
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintPure)
	bool GetIsFocused() const {return bIsFocused; }
	
	UFUNCTION(BlueprintCallable)
	void SetIsFocused(bool InbIsFocused) { bIsFocused =  InbIsFocused; }

	UPROPERTY(BlueprintAssignable)
	FOnReceiveUITaskInUIModeSignature OnReceiveUITask;

	UPROPERTY(BlueprintAssignable)
	FOnOpenedMenuSignature OnOpendMenu;

protected:

	ASoulLikePlayerController* GetSoulLikePC();
	ASoulLikePlayerState* GetSoulLikePS();
	USoulLikeAbilitySystemComponent* GetSoulLikeASC();
	USoulLikeAttributeSet* GetSoulLikeAS();

	UFUNCTION()
	void BroadcastReceiveUITask(const FGameplayTag& TaskTag);

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ASoulLikePlayerController> SoulLikePlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ASoulLikePlayerState> SoulLikePlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<USoulLikeAbilitySystemComponent> SoulLikeAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<USoulLikeAttributeSet> SoulLikeAttributeSet;

	UPROPERTY()
	bool bIsFocused = false;
};
