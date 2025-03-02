// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SoulLikePlayerState.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "Abilitysystem/SoulLikeAttributeSet.h"

ASoulLikePlayerState::ASoulLikePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulLikeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USoulLikeAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ASoulLikePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
