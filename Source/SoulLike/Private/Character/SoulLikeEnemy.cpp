// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeEnemy.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"

ASoulLikeEnemy::ASoulLikeEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulLikeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USoulLikeAttributeSet>("AttributeSet");
}

void ASoulLikeEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void ASoulLikeEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
