// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SoulLikePlayerState.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "Abilitysystem/SoulLikeAttributeSet.h"

#include "Inventory/InventoryComponent.h"

#include "Net/UnrealNetwork.h"

ASoulLikePlayerState::ASoulLikePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulLikeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USoulLikeAttributeSet>("AttributeSet");
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ASoulLikePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASoulLikePlayerState::SetProfileName(FString InProfileName)
{
	ProfileName = InProfileName;
}

void ASoulLikePlayerState::SetPlayerLevel(int32 InLevel)
{
	PlayerLevel = InLevel;
	OnLevelChangedDelegate.Broadcast(PlayerLevel);
	MarkAsDirty();
}

void ASoulLikePlayerState::SetExp(int32 InExp)
{
	Exp = InExp;
	OnExpChangedDelegate.Broadcast(Exp);
	MarkAsDirty();
}

void ASoulLikePlayerState::SetMaxPotion(int32 InMaxPotion)
{
	MaxPotion = InMaxPotion;
	OnMaxPotionChangedDelegate.Broadcast(MaxPotion);
	MarkAsDirty();
}

void ASoulLikePlayerState::AddToPlayerLevel(int32 InLevel)
{
	PlayerLevel += InLevel;
	OnLevelChangedDelegate.Broadcast(PlayerLevel);
	MarkAsDirty();
}

void ASoulLikePlayerState::AddToExp(int32 InExp)
{
	Exp += InExp;
	OnExpChangedDelegate.Broadcast(Exp);
	MarkAsDirty();
}

void ASoulLikePlayerState::AddToMaxPotion(int32 InMaxPotion)
{
	MaxPotion += InMaxPotion;
	OnMaxPotionChangedDelegate.Broadcast(MaxPotion);
	MarkAsDirty();
}

void ASoulLikePlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel)
{
	OnLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ASoulLikePlayerState::OnRep_Exp(int32 OldExp)
{
	OnExpChangedDelegate.Broadcast(Exp);
}

void ASoulLikePlayerState::OnRep_MaxPotionNum(int32 OldMaxPotionNum)
{
	OnMaxPotionChangedDelegate.Broadcast(MaxPotion);
}

bool ASoulLikePlayerState::IsDirty() const
{
	return bIsDirty;
}

void ASoulLikePlayerState::MarkAsClean()
{
	bIsDirty = false;
}

void ASoulLikePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(AbilitySystemComponent))
	{
		SL_ASC->MarkAsDirtyDelegate.BindUObject(this, &ASoulLikePlayerState::MarkAsDirty);
	}

	if(USoulLikeAttributeSet* SL_AS = Cast<USoulLikeAttributeSet>(AttributeSet))
	{
		SL_AS->MarkAsDirtyDelegate.BindUObject(this, &ASoulLikePlayerState::MarkAsDirty);
	}

	if(InventoryComponent)
	{
		InventoryComponent->MarkAsDirtyDelegate.BindUObject(this, &ASoulLikePlayerState::MarkAsDirty);
	}
}

void ASoulLikePlayerState::MarkAsDirty()
{
	bIsDirty = true;
}

void ASoulLikePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulLikePlayerState, PlayerLevel);
	DOREPLIFETIME(ASoulLikePlayerState, Exp);
	DOREPLIFETIME(ASoulLikePlayerState, MaxPotion);
}
