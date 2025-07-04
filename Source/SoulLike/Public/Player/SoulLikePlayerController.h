// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "SoulLikePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class USL_InputConfig;
class USL_UITaskConfig;
class USoulLikeAbilitySystemComponent;
class UUISubSystem;
struct FInputActionValue;

DECLARE_DELEGATE(FOnOpenedMenuWidget);
DECLARE_DELEGATE_OneParam(FOnOpenSavePointMenuWidget, const FString&);

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ASoulLikePlayerController();

	UFUNCTION(BlueprintCallable)
	void SetInputModeTag(FGameplayTag InInputMode);
	bool GetUIMode() const;

	UFUNCTION(BlueprintCallable)
	void OpenSavePointMenu(const FString& SavePointName) const;

	float GetLadderMoveInput() const { return LadderMoveInput; }
	float GetMouseXInput() const { return MouseXInput; }

	FGameplayTag FindInputTagForAbilityTags(const FGameplayTagContainer& AbilityTags) const;
	
	FOnOpenedMenuWidget OnPressedMainMenuButton;
	FOnOpenSavePointMenuWidget OnTriggerSavePointMenu;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UUISubSystem* GetUISubSystem();
	
private:
	
	USoulLikeAbilitySystemComponent* GetASC();
	
	void Move(const FInputActionValue& InputActionValue);
	void LadderMove(const FInputActionValue& InputActionValue);
	void Lock(const FInputActionValue& InputActionValue);
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	void TryUITaskToInputTag(FGameplayTag InputTag);

	UFUNCTION(Server, Reliable)
	void Server_SetLadderMoveInput(float InputValue);

	void ChangeAbilityInputTag(const FGameplayTag& InputTag);

	//UPROPERTY()
	//bool bUIMode = false;

	UPROPERTY()
	FGameplayTag InputMode;
		
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LadderMoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LockAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<USL_InputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<USL_UITaskConfig> UITaskConfig;

	UPROPERTY()
	TObjectPtr<USoulLikeAbilitySystemComponent> SoulLikeAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UUISubSystem> UISubSystem;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	FGameplayTagContainer AntiMovementStatusTags;

	UPROPERTY()
	float MouseXInput = 0.f;
	
	UPROPERTY(Replicated)
	float LadderMoveInput = 0.f;
	
};
