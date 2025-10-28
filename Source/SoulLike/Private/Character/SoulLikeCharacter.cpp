// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacter.h"

#include "Player/SoulLikePlayerState.h"
#include "Player/SoulLikePlayerController.h"

#include "UI/HUD/SoulLikeHUD.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "AbilitySystemComponent.h"

#include "Interface/InteractionInterface.h"

#include "MotionWarpingComponent.h"
#include "OnlineSubsystem.h"
#include "SoulLikeFunctionLibrary.h"

#include "Inventory/InventoryComponent.h"

#include "SoulLikeGameplayTags.h"
#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Character/Data/CharacterDataAsset.h"
#include "Game/AutoSaveSubsystem.h"
#include "Game/ObjectPoolingSubsystem.h"
#include "Game/OnlineSessionSubsystem.h"
#include "Game/SoulLikeGameInstance.h"
#include "Game/SoulLikeGameModeBase.h"
#include "Game/SoulLikeSaveGame.h"
#include "Kismet/GameplayStatics.h"

ASoulLikeCharacter::ASoulLikeCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void ASoulLikeCharacter::CreateOrphanObjects()
{
	OrphanActors.Empty(); // 이전 객체 배열 초기화
	for (int32 i = 0; i < 10; ++i)
	{
		// Owner를 지정했지만 UPROPERTY()로 참조하지 않는 객체 10개를 생성합니다.
		// 이 객체들은 생성된 직후 '고아' 상태가 됩니다.
		AActor* NewOrphan = NewObject<AActor>(this);
		OrphanActors.Add(NewOrphan);
	}
	UE_LOG(LogTemp, Error, TEXT("!!! Created 10 Orphan UObjects. Watch the logs. !!!"));
}

void ASoulLikeCharacter::ForceGarbageCollection()
{
	if (GEngine)
	{
		GEngine->ForceGarbageCollection(true);
		UE_LOG(LogTemp, Error, TEXT("!!! Manually Forced Garbage Collection !!!"));
	}
}

void ASoulLikeCharacter::SetWarpingLocationAndRotation(FVector Location, FRotator Rotation)
{
	if(Location == FVector::ZeroVector)
	{
		Location = GetActorLocation();
	}
	if(Rotation == FRotator::ZeroRotator)
	{
		if(GetCharacterMovement() == nullptr) return;

		if(GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_TargetLock))
		{
			Rotation.Yaw = GetControlRotation().Yaw;
		}
		else
		{
			Rotation = GetActorRotation();
			if(GetCharacterMovement()->GetCurrentAcceleration().Size2D() > 0.f)
			{
				Rotation.Yaw = GetCharacterMovement()->GetCurrentAcceleration().Rotation().Yaw;
			}
		}
	}
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), Location, Rotation);
}

void ASoulLikeCharacter::SetWarpingLocation()
{

}

void ASoulLikeCharacter::SetWarpingRotation(FRotator TargetRotation)
{
	if(TargetRotation != FRotator::ZeroRotator)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), GetActorLocation(), TargetRotation);
		return;
	}
	if(GetCharacterMovement() == nullptr) return;
	
	FRotator Direction = FRotator::ZeroRotator;
	if(GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_TargetLock))
	{
		Direction.Yaw = GetControlRotation().Yaw;
	}
	else
	{
		Direction = GetActorRotation();
		if(GetCharacterMovement()->GetCurrentAcceleration().Size2D() > 0.f)
		{
			Direction.Yaw = GetCharacterMovement()->GetCurrentAcceleration().Rotation().Yaw;
		}
	}
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), GetActorLocation(), Direction);
}

UAnimMontage* ASoulLikeCharacter::GetHitReactMontage_Implementation()
{
	if(USoulLikeGameInstance* GameInstance = USoulLikeFunctionLibrary::GetSL_GameInstance(this))
	{
		if(GameInstance->CharacterDataAsset)
		{
			const TSoftObjectPtr<UAnimMontage>& MontageRef = GameInstance->CharacterDataAsset->PlayerData.HitReactMontage;

			if(!MontageRef.IsValid())
			{
				return MontageRef.LoadSynchronous();
			}
			return MontageRef.Get();
		}
	}
	return nullptr;
}

UAnimMontage* ASoulLikeCharacter::GetRiposteReactMontage_Implementation()
{
	if(USoulLikeGameInstance* GameInstance = USoulLikeFunctionLibrary::GetSL_GameInstance(this))
	{
		if(GameInstance->CharacterDataAsset)
		{
			const TSoftObjectPtr<UAnimMontage>& MontageRef = GameInstance->CharacterDataAsset->PlayerData.RiposteReactMontage;

			if(!MontageRef.IsValid())
			{
				return MontageRef.LoadSynchronous();
			}
			return MontageRef.Get();
		}
	}
	return nullptr;
}

UCameraComponent* ASoulLikeCharacter::GetPlayerCameraComponent() const
{
	return CameraComponent;
}

float ASoulLikeCharacter::GetMouseXInput() const
{
	if(ASoulLikePlayerController* SL_PC = Cast<ASoulLikePlayerController>(GetController()))
	{
		return SL_PC->GetMouseXInput();
	}
	return 0.f;
}

void ASoulLikeCharacter::Pickup(FInventoryData InventoryData)
{
	if(InventoryComponent && HasAuthority())
	{
		InventoryComponent->AddItemToInventoryList(InventoryData);
	}
}

void ASoulLikeCharacter::LoadProgress()
{
	if(!HasAuthority()) return;
	ASoulLikeGameModeBase* SL_GameMode = Cast<ASoulLikeGameModeBase>(UGameplayStatics::GetGameMode(this));
	ASoulLikePlayerState* SL_PlayerState = Cast<ASoulLikePlayerState>(GetPlayerState());
	
	if(UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		ObjectPoolingSubsystem->Init();
	}
	
	if(SL_GameMode && SL_PlayerState)
	{
		if(InventoryComponent)
		{
			InventoryComponent->Init();
		}
		
		USoulLikeSaveGame* SaveData = SL_GameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		if(SaveData->bFirstTimeLoadIn)
		{
			SL_PlayerState->SetProfileName(SaveData->ProfileName);
			InitializeDefaultAttributes();
			GiveAbilities();
			
			if(InventoryComponent)
			{
				InventoryComponent->SetupDefaultInventoryList();
				InventoryComponent->EquipItem();
			}
			RespawnPoint = GetActorTransform();
		}
		else
		{
			if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent))
			{
				SL_ASC->GiveAbilitiesFromSaveData(SaveData, this);
			}
			SL_PlayerState->SetProfileName(SaveData->ProfileName);
			SL_PlayerState->SetPlayerLevel(SaveData->PlayerLevel);
			SL_PlayerState->SetExp(SaveData->EXP);
			SL_PlayerState->SetMaxPotion(SaveData->MaxPotion);
        
			USoulLikeAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);

			if(InventoryComponent)
			{
				InventoryComponent->SetRightWeaponSlotIndex(SaveData->RightWeaponSlotIndex);
				InventoryComponent->SetLeftWeaponSlotIndex(SaveData->LeftWeaponSlotIndex);
				InventoryComponent->SetToolSlotIndex(SaveData->ToolSlotIndex);
				
				InventoryComponent->LoadInventoryListFromSavedItems(SaveData->SavedItems);
			}
    	
			SetActorTransform(SaveData->Transform);
		}
		
		SL_PlayerState->MarkAsClean();
		SL_GameMode->LoadWorldObject(GetWorld());
	}
	if(UAutoSaveSubsystem* AutoSaveSubsystem = GetGameInstance()->GetSubsystem<UAutoSaveSubsystem>())
	{
		AutoSaveSubsystem->Init();
	}
}

void ASoulLikeCharacter::DisableCharacter_Implementation()
{
	Super::DisableCharacter_Implementation();

	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RebirthTimerHandle, this, &ASoulLikeCharacter::Rebirth, 5.f, false);
	}
	InteractionActors.Empty();
	SelectedInteractionActor = nullptr;
}


void ASoulLikeCharacter::InitAbilityActorInfo()
{
	ASoulLikePlayerState* SoulLikePlayerState = GetPlayerState<ASoulLikePlayerState>();
	check(SoulLikePlayerState);

	SoulLikePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SoulLikePlayerState, this);
	AbilitySystemComponent = SoulLikePlayerState->GetAbilitySystemComponent();
	AttributeSet = SoulLikePlayerState->GetAttributeSet();
	InventoryComponent = SoulLikePlayerState->GetInventoryComponent();

	if(ASoulLikePlayerController* SoulLikePlayerController = Cast<ASoulLikePlayerController>(GetController())){
    
		if(ASoulLikeHUD* SoulLikeHUD = Cast<ASoulLikeHUD>(SoulLikePlayerController->GetHUD())){

			SoulLikeHUD->InitOverlay(SoulLikePlayerController, SoulLikePlayerState, AbilitySystemComponent, AttributeSet);
			InventoryComponent->BindToWidgetController();
		}
	}
}

void ASoulLikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASoulLikeCharacter::OnBeginOverlap);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ASoulLikeCharacter::OnEndOverlap);
	}

	// if(UOnlineSessionSubsystem* OSSS = GetGameInstance()->GetSubsystem<UOnlineSessionSubsystem>())
	// {
	// 	OSSS->HostSession(2, FString(""));
	// }
}

void ASoulLikeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	LoadProgress();
}

void ASoulLikeCharacter::OnRep_PlayerState()
{
	InitAbilityActorInfo();
}

void ASoulLikeCharacter::RefreshInventory_Implementation()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateInventoryListToWidgetController();
	}
}

void ASoulLikeCharacter::TriggerSavePointMenuWidget_Implementation()
{
	if(ASoulLikePlayerController* SL_PC = Cast<ASoulLikePlayerController>(GetController()))
	{
		FString InteractionActorName = FString("");
		
		if(SelectedInteractionActor && SelectedInteractionActor->Implements<UInteractionInterface>())
		{
			InteractionActorName = Execute_GetActorName(SelectedInteractionActor);
		}
		else return;
		SL_PC->OpenSavePointMenu(InteractionActorName);
	}
}

FGameplayTag ASoulLikeCharacter::GetInteractionTag_Implementation() const
{
	if(SelectedInteractionActor && SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		return Execute_GetInteractionTag(SelectedInteractionActor);
	}
	return FGameplayTag();
}

void ASoulLikeCharacter::SaveProgress_Implementation() const
{
	if(!IsLocallyControlled()) return;
	ASoulLikeGameModeBase* AuraGameMode = Cast<ASoulLikeGameModeBase>(UGameplayStatics::GetGameMode(this));
	ASoulLikePlayerState* AuraPlayerState = Cast<ASoulLikePlayerState>(GetPlayerState());
	if(!IsValid(AuraGameMode)) return;
    if(AuraGameMode && AuraPlayerState)
    {
        USoulLikeSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
        if(SaveData == nullptr) return;

    	SaveData->bFirstTimeLoadIn = false;	
    	
        if(AuraPlayerState->IsDirty())
        {
        	SaveData->ProfileName = AuraPlayerState->GetProfileName();
	        SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
	        SaveData->EXP = AuraPlayerState->GetExp();
	        SaveData->MaxPotion = AuraPlayerState->GetMaxPotion();
    		
    		SaveData->Vigor = USoulLikeAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
    		SaveData->Mind = USoulLikeAttributeSet::GetMindAttribute().GetNumericValue(GetAttributeSet());
    		SaveData->Endurance = USoulLikeAttributeSet::GetEnduranceAttribute().GetNumericValue(GetAttributeSet());
	        SaveData->Strength = USoulLikeAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
    		SaveData->Dexterity = USoulLikeAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
	        SaveData->Intelligence = USoulLikeAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
    		
	        USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent);
	        FForEachAbility SaveAbilityDelegate;
	        SaveData->SavedAbilities.Empty();
	        SaveAbilityDelegate.BindLambda([this, SL_ASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
	        {
	            const FGameplayTag AbilityTag = SL_ASC->GetAbilityTagFromSpec(AbilitySpec);
	            UAbilityInfo* AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfo(this);
	            FSoulLikeAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	        
	            FSavedAbility SavedAbility;
	            SavedAbility.GameplayAbility = Info.Ability;
	            SavedAbility.InputTag = SL_ASC->GetInputTagFromAbilityTag(AbilityTag);
	            SavedAbility.AbilityTag = AbilityTag;
	            SavedAbility.AbilityType = Info.AbilityType;
	        
	            SaveData->SavedAbilities.AddUnique(SavedAbility);
	        });
	        SL_ASC->ForEachAbility(SaveAbilityDelegate);

    		if(InventoryComponent)
    		{
    			SaveData->SavedItems = InventoryComponent->GetSavedItemFromInventoryList();
    			
    			SaveData->RightWeaponSlotIndex = InventoryComponent->GetRightWeaponSlotIndex();
    			SaveData->LeftWeaponSlotIndex = InventoryComponent->GetLeftWeaponSlotIndex();
    			SaveData->ToolSlotIndex = InventoryComponent->GetToolSlotIndex();
    		}
        }
    	
    	SaveData->Transform = GetActorTransform();

        AuraGameMode->SaveInGameProgressData(SaveData);
    	AuraGameMode->SaveWorldObject(GetWorld());
    	
    	AuraPlayerState->MarkAsClean();
    }
}

void ASoulLikeCharacter::GiveExp_Implementation(int32 ExpValue)
{
	if(ASoulLikePlayerState* SL_PS = Cast<ASoulLikePlayerState>(GetPlayerState()))
	{
		SL_PS->AddToExp(ExpValue);
	}
}

void ASoulLikeCharacter::Interaction_Implementation(AActor* InteractionActor)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	FGameplayTag InteractionTag;

	if(UOverlayWidgetController* OverlayWidgetController = USoulLikeFunctionLibrary::GetOverlayWidgetController(this))
	{
		OverlayWidgetController->OnBeginOverlappedInteractionActorDelegate.Broadcast(nullptr);
	}

	if(SelectedInteractionActor && SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		InteractionTag = Execute_GetInteractionTag(SelectedInteractionActor);
		Execute_Interaction(SelectedInteractionActor, this);
	}
	
	if(InteractionTag.MatchesTagExact(GameplayTags.Interaction_Rest))
	{
		RespawnPoint = GetTransform();
		//SavePointMenu Add To Viewport
		TriggerSavePointMenuWidget_Implementation();

		//Potion Refill
		if(InventoryComponent)
			InventoryComponent->RefillPotion();

		//Enemy Reset
		if(UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
		{
			ObjectPoolingSubsystem->ResetEnemyPool();
		}

		ApplyEffectToSelf(FullHealthEffectClass, 1.f);
		SaveProgress_Implementation();
		
		SetWarpingTargetFromInteractionActor();
	}
	else if(InteractionTag.MatchesTag(GameplayTags.Interaction_Object))
	{
		SetWarpingTargetFromInteractionActor(true);
	}
	else if(InteractionTag.MatchesTag(GameplayTags.Interaction_Ladder))
	{
		SetWarpingTargetFromInteractionActor(true);
	}
	
}

const FInteractionTaskInfo ASoulLikeCharacter::GetInteractionActorInfo_Implementation() const
{
	if(SelectedInteractionActor == nullptr) return FInteractionTaskInfo();
	if(SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		return Execute_GetInteractionActorInfo(SelectedInteractionActor);
	}
	
	return FInteractionTaskInfo();
}

void ASoulLikeCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor->Implements<UInteractionInterface>()) return;
	if(InteractionActors.IsEmpty())
	{
		SelectedInteractionActor = OtherActor;
		
		if(UOverlayWidgetController* OverlayWidgetController = USoulLikeFunctionLibrary::GetOverlayWidgetController(this))
		{
			//const FGameplayTag& InteractionTag = IInteractionInterface::Execute_GetInteractionTag(SelectedInteractionActor);
			OverlayWidgetController->OnBeginOverlappedInteractionActorDelegate.Broadcast(SelectedInteractionActor);
		}
	}
	InteractionActors.AddUnique(OtherActor);
}

void ASoulLikeCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor->Implements<UInteractionInterface>()) return;
	InteractionActors.Remove(OtherActor);
	if(InteractionActors.IsEmpty())
	{
		if(UOverlayWidgetController* OverlayWidgetController = USoulLikeFunctionLibrary::GetOverlayWidgetController(this))
		{
			OverlayWidgetController->OnBeginOverlappedInteractionActorDelegate.Broadcast(nullptr);
		}
		SelectedInteractionActor = nullptr;
	}
}

void ASoulLikeCharacter::SetWarpingTargetFromInteractionActor(bool bHasWarpingPoint)
{
	if(SelectedInteractionActor == nullptr) return;
	
	FVector WarpingLocation = GetActorLocation();
	FRotator WarpingRotation = FRotationMatrix::MakeFromX(SelectedInteractionActor->GetActorLocation() - GetActorLocation()).Rotator();
	WarpingRotation.Pitch = 0.0f;
	WarpingRotation.Roll = 0.0f;

	if(SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		WarpingLocation = Execute_GetWarpingLocation(SelectedInteractionActor);
	}
	
	if(bHasWarpingPoint && SelectedInteractionActor->Implements<UInteractionInterface>())
	{
		WarpingRotation = Execute_GetWarpingRotation(SelectedInteractionActor);
	}
	
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), WarpingLocation, WarpingRotation);
}

void ASoulLikeCharacter::Rebirth()
{
	SetActorTransform(RespawnPoint);
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	if(HasAuthority())
	{
		ApplyEffectToSelf(FullHealthEffectClass, 1.f);
		if(UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
		{
			ObjectPoolingSubsystem->ResetEnemyPool();
		}
		if(InventoryComponent)
		{
			InventoryComponent->EquipItem();
		}
	}
	
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RebirthTimerHandle);
	}
}

