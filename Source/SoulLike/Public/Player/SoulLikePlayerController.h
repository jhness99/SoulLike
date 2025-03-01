// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoulLikePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ASoulLikePlayerController();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:

	void Move(const FInputActionValue& InputActionValue);
	void Lock(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LockAction;
};
