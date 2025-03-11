// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"

ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
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

UAnimMontage* ASoulLikeCharacterBase::GetCurrentWeaponMontage_Implementation()
{
	if(GetCurrentWeapon_Implementation() == nullptr) return nullptr;
	return GetCurrentWeapon_Implementation()->GetMontage();
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

void ASoulLikeCharacterBase::GiveAbilities() const
{
	if(!HasAuthority()) return;
	
	if(USoulLikeAbilitySystemComponent* SoulLikeAbilitySystemComponent = Cast<USoulLikeAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		SoulLikeAbilitySystemComponent->GiveAbilities(StartupAbilities);
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
