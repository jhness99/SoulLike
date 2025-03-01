// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacterBase.h"

// Sets default values
ASoulLikeCharacterBase::ASoulLikeCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASoulLikeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoulLikeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASoulLikeCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

