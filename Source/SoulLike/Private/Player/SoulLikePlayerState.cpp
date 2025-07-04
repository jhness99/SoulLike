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

void ASoulLikePlayerState::SetPlayerLevel(int32 InLevel)
{
	PlayerLevel = InLevel;
	OnLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ASoulLikePlayerState::SetExp(int32 InExp)
{
	Exp = InExp;
	OnExpChangedDelegate.Broadcast(Exp);
}

void ASoulLikePlayerState::SetMaxPotion(int32 InMaxPotion)
{
	MaxPotion = InMaxPotion;
	OnMaxPotionChangedDelegate.Broadcast(MaxPotion);
}

void ASoulLikePlayerState::AddToPlayerLevel(int32 InLevel)
{
	PlayerLevel += InLevel;
	OnLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ASoulLikePlayerState::AddToExp(int32 InExp)
{
	Exp += InExp;
	OnExpChangedDelegate.Broadcast(Exp);
}

void ASoulLikePlayerState::AddToMaxPotion(int32 InMaxPotion)
{
	MaxPotion += InMaxPotion;
	OnMaxPotionChangedDelegate.Broadcast(MaxPotion);
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

void ASoulLikePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulLikePlayerState, PlayerLevel);
	DOREPLIFETIME(ASoulLikePlayerState, Exp);
	DOREPLIFETIME(ASoulLikePlayerState, MaxPotion);
}
