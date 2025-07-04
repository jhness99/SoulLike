// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RollingAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/SoulLikeCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/CombatInterface.h"
#include "SoulLike/SoulLike.h"


void URollingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitEffectEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FSoulLikeGameplayTags::Get().Event_Montage_Effect, nullptr, false, false);
	WaitEffectEventTask->EventReceived.AddDynamic(this, &URollingAbility::OnReceiveEvent);
	WaitEffectEventTask->ReadyForActivation();
}

void URollingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(IsValid(WaitEffectEventTask))
	{
		WaitEffectEventTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URollingAbility::SetupMontage()
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		Montage = ICombatInterface::Execute_EvaluateRollingMontage(GetAvatarActorFromActorInfo());
	}
}

void URollingAbility::MontageJumpToNextCombo()
{
	if(!HasAuthority(&CurrentActivationInfo)) return;
	
	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
	}

	ClearInputTask();
	
	FName Section = GetSectionName();
	
	MontageJumpToSection(Section);
	ResetAbilityState();
}

FName URollingAbility::GetSectionName()
{
	FName Section = FName("Front");
	if(!GetAbilitySystemComponentFromActorInfo_Checked()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_TargetLock)) return Section;
	Section = FName("FrontStrafe");
	
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if(AvatarActor == nullptr) return Section;

	ASoulLikeCharacterBase* SL_Character =  Cast<ASoulLikeCharacterBase>(AvatarActor);
	if(SL_Character == nullptr) return Section;
	if(SL_Character->GetCharacterMovement() == nullptr) return Section;
 	
	FVector AvatarForward = SL_Character->GetActorForwardVector();
	const FVector AccelerationDirection = SL_Character->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal();

	const double CosTheta = FVector::DotProduct(AvatarForward, AccelerationDirection);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(AvatarForward, AccelerationDirection);
	
	if(CrossProduct.Z < 0.f)
	{
		Theta *= -1.f;
	}

	if(Theta < -44.f && Theta >= -134.f)
	{
		Section = FName("Left");
		Theta += 90.f;		
	}
	else if(Theta < -134.f || Theta >= 134.f)
	{
		Section = FName("Back");
		if(Theta < -134.f) Theta += 180.f;
		else if(Theta > 134.f) Theta += -180.f;
	}
	else if(Theta >= 44.f && Theta <= 134.f)
	{
		Section = FName("Right");
		Theta += -90.f;
	}

	FVector WarpingDirection = AvatarForward.RotateAngleAxis(Theta, AvatarActor->GetActorUpVector());

	if(static_cast<bool>(CVarShowRollDirection.GetValueOnAnyThread()))
		DrawDebugDirectionalArrow(GetWorld(), SL_Character->GetActorLocation(), SL_Character->GetActorLocation() + WarpingDirection*150, 10.f, FColor::Red, false, 5.f);
	
	WarpingRotation = WarpingDirection.Rotation();
	SL_Character->SetWarpingRotation(WarpingRotation);
	
	return Section;
}

void URollingAbility::OnReceiveEvent(FGameplayEventData Payload)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if(!ASC) return;

	if(Payload.EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Event_Montage_Effect_Apply))
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ApplyEffectClass, 1.f, ContextHandle);
		
		ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	else if(Payload.EventTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Event_Montage_Effect_Remove))
	{
		ASC->RemoveActiveGameplayEffect(ActiveGEHandle);
	}
}
