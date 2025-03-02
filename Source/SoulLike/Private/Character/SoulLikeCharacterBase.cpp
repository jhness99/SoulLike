// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"


ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ASoulLikeCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASoulLikeCharacterBase::InitAbilityActorInfo()
{
	
}

void ASoulLikeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
