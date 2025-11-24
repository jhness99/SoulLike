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
#include "Character/SoulLikeCharacter.h"
#include "Game/SoulLikeGameInstance.h"

#include "Net/UnrealNetwork.h"
#include "Player/SoulLikePlayerState.h"
#include "UI/HUD/SoulLikeHUD.h"
#include "UI/WidgetController/KeybindMenuWidgetController.h"


ASoulLikePlayerController::ASoulLikePlayerController()
{
	
}

void ASoulLikePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitOverlay();

	if(!IsLocalPlayerController()) return;
	
	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	Server_SendClientSaveData(SL_GameInstance->GetClientSaveData());
}

void ASoulLikePlayerController::SetInputModeTag(const FGameplayTag& InInputMode)
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

void ASoulLikePlayerController::Server_SendClientSaveData_Implementation(const FClientSaveData& SaveDataStruct)
{
	if(ASoulLikeCharacter* SL_Character = Cast<ASoulLikeCharacter>(GetPawn()))
	{
        // 1. (서버에서) 클라이언트 데이터를 담을 "임시" SaveGame 객체를 생성합니다.
        USoulLikeSaveGame* TempSaveGame = NewObject<USoulLikeSaveGame>();
        if (!TempSaveGame)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create TempSaveGame on server."));
            return;
        }

        // 2. RPC로 받은 Struct의 데이터를 임시 SaveGame 객체로 "복사"합니다.
        TempSaveGame->SlotName            = SaveDataStruct.SlotName;
        TempSaveGame->SlotIndex           = SaveDataStruct.SlotIndex;
        TempSaveGame->bFirstTimeLoadIn    = true; // 접속한 클라이언트는 항상 false여야 함
        TempSaveGame->ProfileName         = SaveDataStruct.ProfileName;
        TempSaveGame->PlayerLevel         = SaveDataStruct.PlayerLevel;
        TempSaveGame->EXP                 = SaveDataStruct.EXP;
        TempSaveGame->MaxPotion           = SaveDataStruct.MaxPotion;
        TempSaveGame->Vigor               = SaveDataStruct.Vigor;
        TempSaveGame->Mind                = SaveDataStruct.Mind;
        TempSaveGame->Endurance           = SaveDataStruct.Endurance;
        TempSaveGame->Strength            = SaveDataStruct.Strength;
        TempSaveGame->Dexterity           = SaveDataStruct.Dexterity;
        TempSaveGame->Intelligence        = SaveDataStruct.Intelligence;
        TempSaveGame->SavedAbilities      = SaveDataStruct.SavedAbilities;
        TempSaveGame->SavedItems          = SaveDataStruct.SavedItems;
        TempSaveGame->RightWeaponSlotIndex= SaveDataStruct.RightWeaponSlotIndex;
        TempSaveGame->LeftWeaponSlotIndex = SaveDataStruct.LeftWeaponSlotIndex;
        TempSaveGame->ToolSlotIndex       = SaveDataStruct.ToolSlotIndex;
        //TempSaveGame->Transform           = SaveDataStruct.Transform;
		
        // 3. LoadProgress에는 이 "임시 SaveGame" 객체를 전달합니다.
        SL_Character->LoadProgress(TempSaveGame);
	}
}

void ASoulLikePlayerController::ReturnToClient()
{
	ClientTravel(TEXT("/Game/Blueprints/Map/Dungeon"), TRAVEL_Absolute, false);
	SetInputModeTag(FSoulLikeGameplayTags::Get().InputMode_Game);
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

	// if(IsLocalController() && !HasAuthority())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ASoulLikePlayerController::BeginPlay gameinstance호출"))
	// 	USoulLikeGameInstance* SL_GameInstance = Cast<USoulLikeGameInstance>(GetGameInstance());
	// 	UE_LOG(LogTemp, Warning, TEXT("ASoulLikePlayerController::BeginPlay Server_SendClientSaveData호출"))
	// 	Server_SendClientSaveData(SL_GameInstance->GetClientSaveData());
	// }

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

void ASoulLikePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InitOverlay();
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

void ASoulLikePlayerController::InitOverlay()
{
	if(!IsLocalPlayerController()) return;
	
	if(ASoulLikePlayerState* SL_PS = GetPlayerState<ASoulLikePlayerState>())
	{
		if(ASoulLikeHUD* SoulLikeHUD = Cast<ASoulLikeHUD>(GetHUD()))
		{
			SoulLikeHUD->InitOverlay(this, SL_PS, SL_PS->GetAbilitySystemComponent(), SL_PS->GetAttributeSet());
			SL_PS->GetInventoryComponent()->BindToWidgetController();
			SL_PS->GetInventoryComponent()->UpdateRegistSlotToWidgetController();
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
