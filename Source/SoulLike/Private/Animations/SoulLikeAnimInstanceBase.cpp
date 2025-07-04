// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/SoulLikeAnimInstanceBase.h"

#include "Character/SoulLikeCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "SoulLikeGameplayTags.h"

//GameThread init
void FCharacterBaseAnimInstanceProxy::Initialize(UAnimInstance* InAnimInstance)
{
	Super::Initialize(InAnimInstance);

	if(!InAnimInstance) return;
	
	if(USoulLikeAnimInstanceBase* AnimInstance = Cast<USoulLikeAnimInstanceBase>(GetAnimInstanceObject()))
	{
		Pawn = Cast<ASoulLikeCharacterBase>(AnimInstance->TryGetPawnOwner());
		if(Pawn.Get())
		{
			CharacterMovement = Pawn->GetCharacterMovement();
			
		}
	}
}

//GameThread 1
void FCharacterBaseAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	TRACE_CPUPROFILER_EVENT_SCOPE(FCharacterBaseAnimInstanceProxy::PreUpdate);
	
	if(Pawn.Get())
	{
		Velocity = CharacterMovement->GetLastUpdateVelocity();
		CurrentWeaponItemTypes = Pawn->GetCurrentWeaponItemTypes();
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn.Get());
	}
	if(ASC.Get())
	{
		ASC->GetOwnedGameplayTags(HasGameplayTags);
		bIsTargetLock = ASC->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_TargetLock);
	}
}
 
//AnimationThread 3
void FCharacterBaseAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);

	TRACE_CPUPROFILER_EVENT_SCOPE(FCharacterBaseAnimInstanceProxy::Update);
}

//AnimationThread 2
void USoulLikeAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	TRACE_CPUPROFILER_EVENT_SCOPE(USoulLikeAnimInstanceBase::NativeThreadSafeUpdateAnimation);

	const FCharacterBaseAnimInstanceProxy& Proxy = GetProxyOnAnyThread<FCharacterBaseAnimInstanceProxy>();
	
	Speed = Proxy.Velocity.Size2D();
	Pawn = Proxy.Pawn.Get();
	HasGameplayTags = Proxy.HasGameplayTags;
	CurrentWeaponItemTypes = Proxy.CurrentWeaponItemTypes;
}

