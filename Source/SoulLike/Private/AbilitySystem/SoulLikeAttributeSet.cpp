// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "SoulLikeGameplayTags.h"

#include "GameFramework/Character.h"

#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> USoulLikeAttributeSet::TagsToAttributes;

USoulLikeAttributeSet::USoulLikeAttributeSet()
{
	FSoulLikeGameplayTags GameplayTags = FSoulLikeGameplayTags::Get();

	/**
	 * Primary Attributes
	*/
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Mind, GetMindAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Endurance, GetEnduranceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Stamina, GetStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
}

void USoulLikeAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetHealth(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
}

void USoulLikeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	if(Attribute == GetVigorAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
	if(Attribute == GetMindAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
	if(Attribute == GetEnduranceAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
	if(Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
	if(Attribute == GetDexterityAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
	if(Attribute == GetIntelligenceAttribute())
	{
		NewValue = FMath::Min<float>(NewValue, 99.f);
	}
}

void USoulLikeAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
}

void USoulLikeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Health, OldHealth);
}

void USoulLikeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MaxHealth, OldMaxHealth);
}
	
void USoulLikeAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Stamina, OldStamina);
}
    	
void USoulLikeAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MaxStamina, OldMaxStamina);
}

void USoulLikeAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Vigor, OldVigor);
}

void USoulLikeAttributeSet::OnRep_Mind(const FGameplayAttributeData& OldMind) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Mind, OldMind);
}
	
void USoulLikeAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Endurance, OldEndurance);
}
    	
void USoulLikeAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Strength, OldStrength);
}

void USoulLikeAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Dexterity, OldDexterity);
} 	

void USoulLikeAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, Intelligence, OldIntelligence);
}

void USoulLikeAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC)&& Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceController->GetPawn()))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
	}
}

void USoulLikeAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Mind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}