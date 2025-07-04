// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SoulLikePlayerController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Input/SoulLikeInputComponent.h"
#include "Input/SL_UITaskConfig.h"
#include "EnhancedInputSubsystems.h"

#include "Game/UISubSystem.h"

#include "Interface/PlayerInterface.h"

#include "SoulLikeGameplayTags.h"
#include "SoulLikeFunctionLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

#include "Net/UnrealNetwork.h"
#include "UI/WidgetController/KeybindMenuWidgetController.h"


ASoulLikePlayerController::ASoulLikePlayerController()
{
	
}

void ASoulLikePlayerController::SetInputModeTag(FGameplayTag InInputMode)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	InputMode = InInputMode;
	
	if(InputMode.MatchesTagExact(GameplayTags.InputMode_Game))
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else if(InputMode.MatchesTagExact(GameplayTags.InputMode_UI))
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
	else if(InputMode.MatchesTagExact(GameplayTags.InputMode_KeyBind))
	{
		
	}
}

bool ASoulLikePlayerController::GetUIMode() const
{
	return InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_UI);
}

void ASoulLikePlayerController::OpenSavePointMenu(const FString& SavePointName) const
{
	if(OnTriggerSavePointMenu.IsBound())
	{
		OnTriggerSavePointMenu.Execute(SavePointName);
	}
}

FGameplayTag ASoulLikePlayerController::FindInputTagForAbilityTags(const FGameplayTagContainer& AbilityTags) const
{
	if(InputConfig)
	{
		return InputConfig->FindInputTagForAbilityTags(AbilityTags);
	}
	return FGameplayTag();
}

void ASoulLikePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputContext);
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}

	InputMode = FSoulLikeGameplayTags::Get().InputMode_Game;
}

void ASoulLikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	USoulLikeInputComponent* SL_InputComponent = Cast<USoulLikeInputComponent>(InputComponent);

	SL_InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Move);
	SL_InputComponent->BindAction(LadderMoveAction, ETriggerEvent::Started, this, &ASoulLikePlayerController::LadderMove);
	SL_InputComponent->BindAction(LadderMoveAction, ETriggerEvent::Completed, this, &ASoulLikePlayerController::LadderMove);
	SL_InputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &ASoulLikePlayerController::Lock);
	
	SL_InputComponent->BindAbilityActions(InputConfig, this,
		&ASoulLikePlayerController::AbilityInputTagPressed,
		&ASoulLikePlayerController::AbilityInputTagHeld,
		&ASoulLikePlayerController::AbilityInputTagReleased);
	
}

UUISubSystem* ASoulLikePlayerController::GetUISubSystem()
{
	UISubSystem = USoulLikeFunctionLibrary::GetUISubSystem(this);
	return UISubSystem;
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
	if(GetASC() && GetASC()->HasAnyMatchingGameplayTags(AntiMovementStatusTags))
	{
		return;
	}
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

void ASoulLikePlayerController::LadderMove(const FInputActionValue& InputActionValue)
{
	if(GetASC() && !GetASC()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Ladder))
	{
		LadderMoveInput = 0.f;
		return;
	}
	
	const float InputValue = InputActionValue.Get<float>();
	
	if (FMath::Abs(InputValue - LadderMoveInput) > KINDA_SMALL_NUMBER)
	{
		LadderMoveInput = InputValue;
		Server_SetLadderMoveInput(InputValue);
	}
}

void ASoulLikePlayerController::Lock(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if(GetASC() && GetASC()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_TargetLock))
	{
		MouseXInput = InputAxisVector.X;
		return;
	}
	
	MouseXInput = 0.f;
	
	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y);
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
	}
	
}

void ASoulLikePlayerController::TryUITaskToInputTag(FGameplayTag InputTag)
{
	if(GetUISubSystem())
	{
		const FGameplayTag* TaskTag = UITaskConfig->InputTagToUITask.Find(InputTag);
		if(TaskTag && TaskTag->IsValid())
		{
			UISubSystem->ReceiveUITask(*TaskTag);
		}
	}
}

void ASoulLikePlayerController::ChangeAbilityInputTag(const FGameplayTag& InputTag)
{
	if(UKeybindMenuWidgetController* KeybindMenuWidgetController = USoulLikeFunctionLibrary::GetKeybindMenuWidgetController(this))
	{
		if(GetASC())
		{
			GetASC()->ChangeAbilityInputTag(KeybindMenuWidgetController, InputTag);
		}
	}
}

void ASoulLikePlayerController::Server_SetLadderMoveInput_Implementation(float InputValue)
{
	LadderMoveInput = InputValue;
}

void ASoulLikePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FSoulLikeGameplayTags::Get().InputTag_ESC))
	{
		if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_Game) && GetPawn()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_RefreshInventory(GetPawn());
		}
		if(OnPressedMainMenuButton.IsBound())
		{
			OnPressedMainMenuButton.Execute();
		}
		return;
	}
	
	if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_UI))
	{
		TryUITaskToInputTag(InputTag);
		return;
	}

	if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_KeyBind))
	{
		ChangeAbilityInputTag(InputTag);
		return;
	}
	
	if(GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void ASoulLikePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_UI) ||
		InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_KeyBind)) return;
	
	if(GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void ASoulLikePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_UI) ||
		InputMode.MatchesTagExact(FSoulLikeGameplayTags::Get().InputMode_KeyBind)) return;
	
	if(GetASC())
    {
    	GetASC()->AbilityInputTagReleased(InputTag);
    }
}

void ASoulLikePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulLikePlayerController, LadderMoveInput);
}
