// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SoulLikeGameplayTags.h"

#include "GameFramework/Character.h"
#include "Character/SoulLikeCharacterBase.h"

#include "GameplayEffectExtension.h"

#include "Perception/AISense_Damage.h"

#include "Net/UnrealNetwork.h"


TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> USoulLikeAttributeSet::TagsToAttributes;

USoulLikeAttributeSet::USoulLikeAttributeSet()
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();

	/**
	 * Primary Attributes
	*/
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Mind, GetMindAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Endurance, GetEnduranceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Stamina, GetStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StaminaRegeneration, GetStaminaRegenerationAttribute);

	/**
	 * Boost Attribute
	 */
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_HealthBoost, GetHealthBoostAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_StaminaBoost, GetStaminaBoostAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Boost_DamageBoost, GetDamageBoostAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_PhysicalDamageBoost, GetPhysicalDamageBoostAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_PhysicalDamageBoost, GetFireDamageBoostAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_PhysicalDamageBoost, GetMagicDamageBoostAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Boost_PhysicalDamageBoost, GetLightningDamageBoostAttribute);
}

void USoulLikeAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
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
	if(Attribute == GetMaxStaminaAttribute() && bTopOffStamina)
	{
	    SetStamina(GetMaxStamina());
	    bTopOffStamina = false;
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

void USoulLikeAttributeSet::OnRep_DamageBoost(const FGameplayAttributeData& OldDamageBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, DamageBoost, OldDamageBoost);
}

void USoulLikeAttributeSet::OnRep_PhysicalDamageBoost(const FGameplayAttributeData& OldPhysicalDamageBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, PhysicalDamageBoost, OldPhysicalDamageBoost);
}

void USoulLikeAttributeSet::OnRep_FireDamageBoost(const FGameplayAttributeData& OldFireDamageBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, FireDamageBoost, OldFireDamageBoost);
}

void USoulLikeAttributeSet::OnRep_MagicDamageBoost(const FGameplayAttributeData& OldMagicDamageBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MagicDamageBoost, OldMagicDamageBoost);
}

void USoulLikeAttributeSet::OnRep_LightningDamageBoost(const FGameplayAttributeData& OldLightningDamageBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, LightningDamageBoost, OldLightningDamageBoost);
}

void USoulLikeAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MaxStamina, OldMaxStamina);
}

void USoulLikeAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
}

void USoulLikeAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, FireResistance, OldFireResistance);
}

void USoulLikeAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, LightningResistance, OldLightningResistance);
}

void USoulLikeAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, MagicResistance, OldMagicResistance);
}

void USoulLikeAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, FireResistance, OldFireResistance);
}

void USoulLikeAttributeSet::OnRep_HealthBoost(const FGameplayAttributeData& OldHealthBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, HealthBoost, OldHealthBoost);
}

void USoulLikeAttributeSet::OnRep_StaminaBoost(const FGameplayAttributeData& OldStaminaBoost) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulLikeAttributeSet, StaminaBoost, OldStaminaBoost);
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
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
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

		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void USoulLikeAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	
	if(GetOwningAbilitySystemComponent() && GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Invincibility))
	{
		return;
	}
	
	if(LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatel = NewHealth <= 0.f;
		if(bFatel)
		{
			if(ASoulLikeCharacterBase* SL_Character = Cast<ASoulLikeCharacterBase>(Props.TargetCharacter))
			{
				SL_Character->DeathToCharacter(Props.SourceCharacter);	
			}
			bTopOffHealth = true;
			bTopOffStamina = true;
		}
		else
		{
			FGameplayEventData Payload;
			Payload.Instigator = Props.SourceCharacter;
			Payload.EventTag = FSoulLikeGameplayTags::Get().Abilities_Stagger_HitReact;
			if(Props.TargetASC && Props.TargetASC->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_HitReact))
			{
				Payload.EventTag = FSoulLikeGameplayTags::Get().Abilities_Stagger_HitReact_Trigger;
			}
			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.TargetCharacter, Payload.EventTag, Payload);
			//Multicast_SendHitReactTriggerEvent(Props.TargetCharacter, Payload);
			if(ASoulLikeCharacterBase* SL_Character = Cast<ASoulLikeCharacterBase>(Props.TargetCharacter))
			{
				SL_Character->Multicast_SendHitReactTriggerEvent(Payload);
			}

			UAISense_Damage::ReportDamageEvent(
				this,
				Props.TargetCharacter,
				Props.SourceCharacter,
				LocalIncomingDamage,
				Props.TargetCharacter->GetActorLocation(),
				Props.TargetCharacter->GetActorLocation());
			}
	}
}

void USoulLikeAttributeSet::Multicast_SendHitReactTriggerEvent_Implementation(AActor* Target, const FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, Payload.EventTag, Payload);
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
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, HealthBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, StaminaBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, DamageBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, PhysicalDamageBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, FireDamageBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, MagicDamageBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulLikeAttributeSet, LightningDamageBoost, COND_None, REPNOTIFY_Always);
}
