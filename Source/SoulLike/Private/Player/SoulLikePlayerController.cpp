// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SoulLikePlayerController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Input/SoulLikeInputComponent.h"
#include "EnhancedInputSubsystems.h"


ASoulLikePlayerController::ASoulLikePlayerController()
{
	
}

void ASoulLikePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputContext);
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}
}

void ASoulLikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	USoulLikeInputComponent* SL_InputComponent = Cast<USoulLikeInputComponent>(InputComponent);

	SL_InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Move);
	SL_InputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Lock);
	
	SL_InputComponent->BindAbilityActions(InputConfig, this,
		&ASoulLikePlayerController::AbilityInputTagPressed,
		&ASoulLikePlayerController::AbilityInputTagHeld,
		&ASoulLikePlayerController::AbilityInputTagReleased);
}

USoulLikeAbilitySystemComponent* ASoulLikePlayerController::GetASC()
{
	if(SoulLikeAbilitySystemComponent == nullptr)
	{
		SoulLikeAbilitySystemComponent = Cast<USoulLikeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return SoulLikeAbilitySystemComponent;
}

void ASoulLikePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ASoulLikePlayerController::Lock(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y);
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
	}
}

void ASoulLikePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pressed Input Tag is %s"), *InputTag.GetTagName().ToString());
	if(GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void ASoulLikePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Warning, TEXT("Held Input Tag is %s"), *InputTag.GetTagName().ToString());
	if(GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void ASoulLikePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Warning, TEXT("Released Input Tag is %s"), *InputTag.GetTagName().ToString());
	if(GetASC())
    {
    	GetASC()->AbilityInputTagReleased(InputTag);
    }
}
