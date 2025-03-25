// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"
#include "Character/Data/CharacterDataAsset.h"

#include "Game/SoulLikeGameInstance.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"

#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SoulLike/SoulLike.h"
#include "Kismet/GameplayStatics.h"

ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	MotionWarpingComponent->SetIsReplicated(true);

	GetCharacterMovement()->SetIsReplicated(true);
	
	SetReplicates(true);
	SetReplicatingMovement(true);
}

UAbilitySystemComponent* ASoulLikeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASoulLikeCharacterBase::EquipOnCharacter_Implementation(AActor* Equipment)
{
	Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
}

UInventoryItemInstance* ASoulLikeCharacterBase::GetCurrentWeapon_Implementation()
{
	if(InventoryComponent == nullptr) return nullptr;

	return InventoryComponent->GetCurrentWeapon();
}

UItemData* ASoulLikeCharacterBase::GetCurrentWeaponItemData_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetItemData();
}

UAnimMontage* ASoulLikeCharacterBase::GetCurrentWeaponMontage_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetMontage();
}

UAnimMontage* ASoulLikeCharacterBase::EvaluateRollingMontage_Implementation()
{
	if(USoulLikeGameInstance* Instance = Cast<USoulLikeGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if(Instance->CharacterDataAsset)
		{
			return Instance->CharacterDataAsset->PlayerData.MiddleRoll;
		}
	}
	return nullptr;
}


void ASoulLikeCharacterBase::NextSlot_Implementation(EWeaponSlot WeaponSlot)
{
	if(!HasAuthority()) return;
	if(InventoryComponent == nullptr) return;
	InventoryComponent->NextSlot(WeaponSlot);
}

void ASoulLikeCharacterBase::MeleeTrace_Implementation(const FVector& TraceStart, const FVector& TraceEnd, float Radius)
{
	ServerMeleeTrace_Implementation(TraceStart, TraceEnd, Radius);
}

void ASoulLikeCharacterBase::ClearIgnoreActors_Implementation()
{
	if(IgnoreActors.Num() == 0) return;

	IgnoreActors.Empty();
}

void ASoulLikeCharacterBase::SetWarpingLocationAndRotation_Implementation()
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WarpTarget"), GetActorLocation(), GetActorRotation());
}

void ASoulLikeCharacterBase::ServerMeleeTrace_Implementation(const FVector& TraceStart, const FVector& TraceEnd, float Radius)
{
	TArray<FHitResult> HitResults;

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

	if (HitResults.Num() > 0)
	{
		DrawDebugCapsule(GetWorld(), Center, TraceVector.Length() * 0.5, Radius, CapsuleRot, FColor::Green, false, 5);
		
	}

	for(FHitResult& HitResult : HitResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerTrace Hit Target is %s"), *HitResult.GetActor()->GetName());
		IgnoreActors.Add(HitResult.GetActor());
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
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	//AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ASoulLikeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}
