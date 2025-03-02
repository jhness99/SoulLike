// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "Net/UnrealNetwork.h"

void USoulLikeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const{

	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Health, OldHealth);
}

void USoulLikeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const{

	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MaxHealth, OldMaxHealth);
}
	
void USoulLikeAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const{

	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Stamina, OldStamina);
}
    	
void USoulLikeAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const{

	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MaxStamina, OldMaxStamina);
}

void USoulLikeAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}