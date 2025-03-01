// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"


ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASoulLikeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulLikeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoulLikeCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

