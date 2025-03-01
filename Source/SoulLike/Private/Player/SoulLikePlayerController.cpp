// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SoulLikePlayerController.h"

#include "EnhancedInputComponent.h"
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

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Move);
	EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Lock);
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
