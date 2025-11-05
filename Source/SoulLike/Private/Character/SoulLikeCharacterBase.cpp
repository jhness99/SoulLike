// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"
#include "Character/Data/CharacterDataAsset.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/SoulLikePlayerController.h"

#include "Components/WidgetComponent.h"

#include "Game/SoulLikeGameInstance.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/EquipmentItemInstance.h"

#include "MotionWarpingComponent.h"
#include "CharacterTrajectoryComponent.h"
#include "GameplayEffectExtension.h"
#include "Animations/SoulLikeAnimInstanceBase.h"

#include "SoulLike/SoulLike.h"
#include "Kismet/GameplayStatics.h"
#include "SoulLikeFunctionLibrary.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Character/SoulLikeEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/ItemActor.h"
#include "UI/Widgets/SoulLikeUserWidget.h"


ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	MotionWarpingComponent->SetIsReplicated(true);

	CharacterTrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>("CharacterTrajectoryComponent");
	CharacterTrajectoryComponent->SetIsReplicated(true);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());

	TargetLockMarkWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("TargetLockMarkWidgetComponent");

	GetCharacterMovement()->SetIsReplicated(true);
	
	SetReplicates(true);
	SetReplicatingMovement(true);
}

UAbilitySystemComponent* ASoulLikeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASoulLikeCharacterBase::EquipOnCharacter_Implementation(AActor* Equipment, FName SocketName)
{
	Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

UEquipmentItemInstance* ASoulLikeCharacterBase::GetCurrentWeapon_Implementation()
{
	if(InventoryComponent == nullptr) return nullptr;

	return InventoryComponent->GetCurrentWeapon();
}

UItemData* ASoulLikeCharacterBase::GetCurrentWeaponItemData_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetItemData();
}

void ASoulLikeCharacterBase::SetupCurrentWeaponItemType_Implementation(const FGameplayTag& ItemType)
{
	CurrentWeaponItemTypes.Reset();
	CurrentWeaponItemTypes.AddTag(ItemType);
}

UAnimMontage* ASoulLikeCharacterBase::GetCurrentWeaponAttackMontage_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetAttackMontage();
}

UAnimMontage* ASoulLikeCharacterBase::GetCurrentWeaponRiposteMontage_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetRiposteMontage();
}

UEquipmentItemInstance* ASoulLikeCharacterBase::GetCurrentTool_Implementation()
{
	if(InventoryComponent == nullptr) return nullptr;

	return InventoryComponent->GetCurrentTool();
}

UAnimMontage* ASoulLikeCharacterBase::GetCurrentToolMontage_Implementation()
{
	if(GetCurrentTool_Implementation() == nullptr) return nullptr;
	return GetCurrentTool_Implementation()->GetUsingMontage();
}

UAnimMontage* ASoulLikeCharacterBase::EvaluateRollingMontage_Implementation()
{
	if(USoulLikeGameInstance* GameInstance = USoulLikeFunctionLibrary::GetSL_GameInstance(this))
	{
		if(GameInstance->CharacterDataAsset)
		{
			return GameInstance->CharacterDataAsset->PlayerData.MiddleRoll;
		}
	}
	return nullptr;
}

void ASoulLikeCharacterBase::UsingTool_Implementation(URegisterableItemInstance* ItemInstance)
{
	if(InventoryComponent == nullptr) return;
	
	InventoryComponent->UsingTool(ItemInstance);
	if(ItemInstance != nullptr)
	{
		UToolData* ToolData = ItemInstance->GetToolData();
		if(ToolData->ToolActionType == EToolActionType::ETAT_Effect)
		{
			if(ToolData->UsingEffect->IsValidLowLevel())
			{
				ApplyEffectToSelf(ToolData->UsingEffect, 1);
			}
		}
		else if(ToolData->ToolActionType == EToolActionType::ETAT_Ability)
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(ToolData->UsingAbilityTag);
			
			bool result = GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTags);
			if(result)
				UE_LOG(LogTemp, Warning, TEXT("White Sign : true"));
		}
	}
}

void ASoulLikeCharacterBase::NextSlot_Implementation(const FGameplayTag& SlotTag)
{
	if(!HasAuthority()) return;
	if(InventoryComponent == nullptr) return;
	InventoryComponent->NextSlot(SlotTag);
}

void ASoulLikeCharacterBase::MeleeTrace_Implementation(const FTransform& TraceStart, const FTransform& TraceEnd, float Radius)
{
	if(GetLocalRole() == ROLE_SimulatedProxy)
	{
		ClientMeleeTrace(TraceStart, TraceEnd, Radius);
	}
	else
	{
		ServerMeleeTrace(TraceStart, TraceEnd, Radius);
	}
}

void ASoulLikeCharacterBase::ApplyDamageToTarget_Implementation(AActor* Target, float DamageBoost)
{
	Server_ApplyDamageToTarget(Target, DamageBoost);
}

void ASoulLikeCharacterBase::ClearIgnoreActors_Implementation()
{
	if(IgnoreActors.Num() == 0) return;

	IgnoreActors.Empty();
}

void ASoulLikeCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if(GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->GetOwnedGameplayTags(TagContainer);
	}
}

bool ASoulLikeCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(GetAbilitySystemComponent())
	{
		return GetAbilitySystemComponent()->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool ASoulLikeCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(GetAbilitySystemComponent())
	{
		return GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}

void ASoulLikeCharacterBase::SetWarpingLocationAndRotation(FVector Location, FRotator Rotation)
{
	if(Location == FVector::ZeroVector)
	{
		Location = GetActorLocation();
	}
	if(Rotation == FRotator::ZeroRotator)
	{
		Rotation = GetActorRotation();
	}
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), Location, Rotation);
}

void ASoulLikeCharacterBase::SetWarpingLocation()
{
	
}

void ASoulLikeCharacterBase::SetWarpingRotation(FRotator TargetRotation)
{
	if(TargetRotation != FRotator::ZeroRotator)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), GetActorLocation(), TargetRotation);
	}
}

bool ASoulLikeCharacterBase::TryBackstab_Implementation()
{
	if(!GetCurrentWeapon_Implementation()) return false;
	
	if(UWorld* World = GetWorld())
	{
		TArray<FHitResult> HitResults;

		const FVector CharacterForwardVector = GetActorForwardVector();
		
		FCollisionQueryParams Params = FCollisionQueryParams();
		Params.AddIgnoredActor(this);
		
		bool Check = World->SweepMultiByChannel(
			HitResults,
			GetActorLocation(),
			GetActorLocation() + CharacterForwardVector * 100.f,
			FQuat::Identity,
			ECC_AttackTrace,
			FCollisionShape::MakeSphere(25.f),
			Params);
		
		if(HitResults.Num() > 0)
		{
			FVector TraceVector = (HitResults[0].TraceEnd - HitResults[0].TraceStart);
			FVector Center = HitResults[0].TraceStart + (TraceVector * 0.5f);
			FQuat CapsuleRot = FRotationMatrix::MakeFromZ(HitResults[0].TraceEnd - HitResults[0].TraceStart).ToQuat();
			
			if(static_cast<bool>(CVarShowBackstabTrace.GetValueOnAnyThread()))
				DrawDebugCapsule(GetWorld(), Center, TraceVector.Length() * 0.5, 25.f, CapsuleRot, FColor::Green, false, 5);
			
			if(AActor* ClosestActor = USoulLikeFunctionLibrary::ClosestActorWithHitResults(this, HitResults))
			{
				UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ClosestActor);
				if(ASC == nullptr) return false;
				if(ASC->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Riposte)) return false;
				
				FGameplayEventData Payload;
				Payload.Instigator = this;
				
				const FVector ClosestActorForwardVector = ClosestActor->GetActorForwardVector();

				float CosTheta = FVector::DotProduct(ClosestActorForwardVector, CharacterForwardVector);
				float Theta = FMath::Acos(CosTheta);

				Theta = FMath::RadiansToDegrees(Theta);

				if(Theta < 20.f)
				{
					Payload.Target = ClosestActor;
					
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FSoulLikeGameplayTags::Get().Abilities_Backstab,Payload);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ClosestActor, FSoulLikeGameplayTags::Get().Abilities_Backstab,Payload);
					return true;
				}
				
				if(ASC && ASC->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Groggy))
				{
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FSoulLikeGameplayTags::Get().Abilities_Riposte,Payload);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ClosestActor, FSoulLikeGameplayTags::Get().Abilities_Riposte,Payload);
					return true;
				}
			}
		}
	}
	return false;
}

void ASoulLikeCharacterBase::SetStrafeMode_Implementation(bool bStrafe)
{
	Server_SetStrafeMode(bStrafe);
}

void ASoulLikeCharacterBase::SetControllerRotation(const FRotator& Rotation) const
{
	if(GetController())
	{
		GetController()->SetControlRotation(Rotation);
	}
}

void ASoulLikeCharacterBase::Server_SetStrafeMode_Implementation(bool bStrafe)
{
	Multicast_SetStrafeMode(bStrafe);
}

void ASoulLikeCharacterBase::Multicast_SetStrafeMode_Implementation(bool bStrafe)
{
	if(GetCharacterMovement() == nullptr) return;
	
	GetCharacterMovement()->bOrientRotationToMovement  = !bStrafe;
	GetCharacterMovement()->bUseControllerDesiredRotation  = bStrafe;
}

bool ASoulLikeCharacterBase::GetLadderMirror_Implementation() const
{
	if(USoulLikeAnimInstanceBase* AnimInstance = GetSL_AnimInstance())
	{
		return AnimInstance->GetIdleMirror();
	}
	return false;
}

void ASoulLikeCharacterBase::SetLadderMirror_Implementation(bool InMirror)
{
	if(USoulLikeAnimInstanceBase* AnimInstance = GetSL_AnimInstance())
	{
		AnimInstance->SetIdleMirror(InMirror);
	}
}

bool ASoulLikeCharacterBase::GetMirror_Implementation() const
{
	if(USoulLikeAnimInstanceBase* AnimInstance = GetSL_AnimInstance())
	{
		return AnimInstance->GetMirror();
	}
	return false;
}

void ASoulLikeCharacterBase::SetMirror_Implementation(bool InMirror)
{
	if(USoulLikeAnimInstanceBase* AnimInstance = GetSL_AnimInstance())
	{
		AnimInstance->SetMirror(InMirror);
	}
}

void ASoulLikeCharacterBase::SetMovementMode_Implementation(EMovementMode MovementMode)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MovementMode);
	}
}

float ASoulLikeCharacterBase::GetLadderMoveInput_Implementation() const
{
	if(ASoulLikePlayerController* SL_PC = Cast<ASoulLikePlayerController>(GetController()))
	{
		return SL_PC->GetLadderMoveInput();
	}
	return 0.f;
}

void ASoulLikeCharacterBase::ToggleTargetMark_Implementation()
{
	if(TargetLockMarkWidgetComponent == nullptr)  return;
	if(TargetLockMarkWidgetComponent->GetUserWidgetObject() ==  nullptr) return;

	ESlateVisibility Visibility = TargetLockMarkWidgetComponent->GetUserWidgetObject()->GetVisibility();

	if(Visibility == ESlateVisibility::Hidden)
	{
		TargetLockMarkWidgetComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		if(HealthWidgetComponent->IsActive())
		{
			OnChangedVisible.Broadcast(true,  0.f);
		}
	}
	else
	{
		TargetLockMarkWidgetComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		OnChangedVisible.Broadcast(false, 2.f);
	}
}



void ASoulLikeCharacterBase::InitAbilityActorInfo()
{
	
}

void ASoulLikeCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
	ApplyEffectToSelf(DefaultVitalRegeneration, 1);
}

void ASoulLikeCharacterBase::GiveAbilities()
{
	if(!HasAuthority()) return;
	
	if(USoulLikeAbilitySystemComponent* SoulLikeAbilitySystemComponent = Cast<USoulLikeAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		SoulLikeAbilitySystemComponent->GiveAbilities(StartupAbilities, this);
	}
}

void ASoulLikeCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	// check(IsValid(GetAbilitySystemComponent()));
	// check(GameplayEffectClass);
	//
	// FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	// ContextHandle.AddSourceObject(this);
	// const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	// AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent))
	{
		SL_ASC->ApplyEffectToSelf(GameplayEffectClass, Level);
	}
}

void ASoulLikeCharacterBase::SetupDamageParams(FDamageEffectParams& DamageEffectParams, float DamageBoost)
{
	if(UEquipmentItemInstance* CurrentWeapon = GetCurrentWeapon_Implementation())
	{
		if(UWeaponData* CurrentWeaponData = CurrentWeapon->GetWeaponData())
		{
			DamageEffectParams.DamageInfos = CurrentWeaponData->DamageInfos;
			DamageEffectParams.AttackType = CurrentWeaponData->DamageType;
			DamageEffectParams.AttackForce = CurrentWeaponData->AttackForce;
			DamageEffectParams.Toughness = CurrentWeaponData->Toughness;
		}
	}
	
	for(FWeaponDamageInfo& DamageInfo : DamageEffectParams.DamageInfos)
	{
		DamageInfo.Damage *= DamageBoost;
	}
}

void ASoulLikeCharacterBase::TryMeleeTrace(const FTransform& TraceStartRelativeTransform,
	const FTransform& TraceEndRelativeTransform, float Radius)
{
	const bool bDebug = static_cast<bool>(CVarShowAttackTrace.GetValueOnAnyThread());

	TArray<FHitResult> HitResults;

	FTransform TraceStartTransform = TraceStartRelativeTransform * GetActorTransform();
	FTransform TraceEndTransform = TraceEndRelativeTransform * GetActorTransform();

	FVector TraceStart = TraceStartTransform.GetLocation();
	FVector TraceEnd = TraceEndTransform.GetLocation();
	
	FRotator Direction = (TraceEnd - TraceStart).Rotation();
	FCollisionQueryParams Params = FCollisionQueryParams();
	
	Params.AddIgnoredActors(IgnoreActors);
	Params.AddIgnoredActor(this);
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		Direction.Quaternion(),
		ECC_AttackTrace,
		FCollisionShape::MakeSphere(Radius),
		Params);

	FVector TraceVector = (TraceEnd - TraceStart);
	FVector Center = TraceStart + (TraceVector * 0.5f);
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceEnd - TraceStart).ToQuat();

	if(bDebug)
	{
		if (HitResults.Num() > 0)
		{
			DrawDebugCapsule(GetWorld(), Center, TraceVector.Length() * 0.5, Radius, CapsuleRot, FColor::Green, false, 5);
		}
		else
		{
			DrawDebugCapsule(GetWorld(), Center, TraceVector.Length() * 0.5, Radius, CapsuleRot, FColor::Red, false, 5);
		}
	}
	
	FDamageEffectParams DamageEffectParams;

	DamageEffectParams.WorldContextObject = this;
	DamageEffectParams.DamageGameplayEffectClass = DamageGameplayEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponent();

	SetupDamageParams(DamageEffectParams);

	for(FHitResult& HitResult : HitResults)
	{
		if(HitResult.GetActor() == nullptr) continue;
		
		IgnoreActors.Add(HitResult.GetActor());

		USoulLikeAbilitySystemComponent* TargetASC = Cast<USoulLikeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor()));
		if(TargetASC && TargetASC->HasAnyMatchingGameplayTags(HitImmunityStateTags)) return;
		
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		DamageEffectParams.KnockbackForce = (HitResult.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		TargetASC->Server_ApplyDamageEffect(DamageEffectParams);
	}
}

void ASoulLikeCharacterBase::ClientMeleeTrace_Implementation(const FTransform& TraceStartRelativeTransform,
                                                             const FTransform& TraceEndRelativeTransform, float Radius)
{
	TryMeleeTrace(TraceStartRelativeTransform, TraceEndRelativeTransform, Radius);
}

void ASoulLikeCharacterBase::ServerMeleeTrace_Implementation(const FTransform& TraceStartRelativeTransform, const FTransform& TraceEndRelativeTransform, float Radius)
{
	TryMeleeTrace(TraceStartRelativeTransform, TraceEndRelativeTransform, Radius);
}

USoulLikeAnimInstanceBase* ASoulLikeCharacterBase::GetSL_AnimInstance() const
{
	if(GetMesh() && GetMesh()->GetAnimInstance())
	{
		return Cast<USoulLikeAnimInstanceBase>(GetMesh()->GetAnimInstance());
	}
	return nullptr;
}

void ASoulLikeCharacterBase::SwitchWalkSpeed_Implementation() const
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;;
	}
}

void ASoulLikeCharacterBase::SwitchRunSpeed_Implementation() const
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void ASoulLikeCharacterBase::SetWeaponCollisionEnable_Implementation(bool bEnable, EMeleeTraceType MeleeTraceTyp, FName InAbilityIndex)
{
	if(InventoryComponent && InventoryComponent->GetCurrentWeapon())
	{
		InventoryComponent->GetCurrentWeapon()->SetCollisionEnable(bEnable);

		AbilityIndex = InAbilityIndex;
	}
	if(!bEnable)
	{
		ClearIgnoreActors_Implementation();
		AbilityIndex = FName("");
	}
}

FOnDeathSignature& ASoulLikeCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

void ASoulLikeCharacterBase::Server_ApplyDamageToTarget_Implementation(AActor* Target, float DamageBoost)
{
	FDamageEffectParams DamageEffectParams;

	DamageEffectParams.WorldContextObject = this;
	DamageEffectParams.DamageGameplayEffectClass = DamageGameplayEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponent();
	
	SetupDamageParams(DamageEffectParams, DamageBoost);

	USoulLikeAbilitySystemComponent* TargetASC = Cast<USoulLikeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
	if(!TargetASC) return;
		
	DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
	DamageEffectParams.KnockbackForce = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	if(GetLocalRole() == ROLE_SimulatedProxy)
	{
		TargetASC->Server_ApplyDamageEffect(DamageEffectParams);
	}
	else
	{
		USoulLikeAbilitySystemComponent* SourceASC = Cast<USoulLikeAbilitySystemComponent>(GetAbilitySystemComponent());
		if(!SourceASC) return;

		SourceASC->Server_ApplyDamageEffect(DamageEffectParams);
	}
}

void ASoulLikeCharacterBase::Multicast_SendHitReactTriggerEvent_Implementation(
	const FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Payload.EventTag, Payload);
}

void ASoulLikeCharacterBase::DeathToCharacter(AActor* InstigatorActor)
{
	if(GetMesh() && GetMesh()->GetAnimInstance())
	{
		if(HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Riposte) &&
			!GetMesh()->GetAnimInstance()->OnMontageEnded.IsBound())
		{
			GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASoulLikeCharacterBase::OnDeathMontageEnd);
			return;
		}
	}
	DisableCharacter();
}

void ASoulLikeCharacterBase::DisableCharacter_Implementation()
{
	if(HasAuthority()) GetAbilitySystemComponent()->CancelAllAbilities();
	
	if(GetCurrentWeapon_Implementation())
	{
		if(AItemActor* WeaponActor = GetCurrentWeapon_Implementation()->GetItemActor())
		{
			WeaponActor->DetachAndDestroy();
		}
	}
	
	//TODO
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IgnoreActors.Empty();
	
	OnDeath.Broadcast();
}

void ASoulLikeCharacterBase::OnDeathMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	DisableCharacter();
}

FGameplayTagContainer ASoulLikeCharacterBase::GetCurrentWeaponItemTypes() const
{
	return CurrentWeaponItemTypes;
}

void ASoulLikeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetMesh())
		TargetLockMarkWidgetComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale ,FName("TargetLockMark"));

	if(HealthWidgetComponent)
	{
		HealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()+20.f));
	}

	if(USoulLikeUserWidget* UserWidget = Cast<USoulLikeUserWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}

	if(const USoulLikeAttributeSet* SoulLikeAS = Cast<USoulLikeAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoulLikeAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoulLikeAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		
		if(GetController() != UGameplayStatics::GetPlayerController(this, 0))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SoulLikeAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				if(Data.GEModData && Data.GEModData->EvaluatedData.ModifierOp == EGameplayModOp::Type::Override) return;
				if(Data.OldValue != 0.f && Data.OldValue > Data.NewValue)
				{
					if(HealthWidgetComponent->IsActive())
					{
						OnChangedVisible.Broadcast(true, 2.f);
					}
				}
			});
		}

		OnMaxHealthChanged.Broadcast(SoulLikeAS->GetMaxHealth());
		OnHealthChanged.Broadcast(SoulLikeAS->GetHealth());
	}
}

float ASoulLikeCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
