// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Rolling.h"

#include "Interface/CombatInterface.h"


void UGA_Rolling::SetupMontage()
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		Montage = ICombatInterface::Execute_EvaluateRollingMontage(GetAvatarActorFromActorInfo());
	}
}

void UGA_Rolling::MontageJumpToNextCombo()
{
	FName Section = FName("Front");
	MontageJumpToSection(Section);
	ResetAbilityState();
}
