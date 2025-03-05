// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeCharacter.h"

#include "Player/SoulLikePlayerState.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "AbilitySystemComponent.h"


ASoulLikeCharacter::ASoulLikeCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void ASoulLikeCharacter::InitAbilityActorInfo()
{
	ASoulLikePlayerState* SoulLikePlayerState = GetPlayerState<ASoulLikePlayerState>();
	check(SoulLikePlayerState);

	SoulLikePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SoulLikePlayerState, this);
	AbilitySystemComponent = SoulLikePlayerState->GetAbilitySystemComponent();
	AttributeSet = SoulLikePlayerState->GetAttributeSet();
}

void ASoulLikeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	InitializeDefaultAttributes();

	GiveAbilities();
}

void ASoulLikeCharacter::OnRep_PlayerState()
{
	InitAbilityActorInfo();
}
