// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/SoulLikeGameplayAbility.h"

#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SoulLikeFunctionLibrary.h"
#include "Game/SoulLikeSaveGame.h"

#include "UI/WidgetController/KeybindMenuWidgetController.h"


void USoulLikeAbilitySystemComponent::GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, APawn* Instigator)
{
	for(const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if(AbilitySpec.Ability)
		{
			FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
			FSoulLikeAbilityInfo SoulLikeAbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfoForTag(this, AbilityTag);
			FGameplayTag InputTag = SoulLikeAbilityInfo.InputTag;
			if(InputTag.IsValid())
				AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		}
		AbilitySpec.SourceObject = Instigator;
		GiveAbility(AbilitySpec);
	}
}

void USoulLikeAbilitySystemComponent::GiveAbilitiesFromSaveData(USoulLikeSaveGame* SaveData, APawn* Instigator)
{
	for(const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = Data.GameplayAbility;

		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, 1.f);

		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.InputTag);
		LoadedAbilitySpec.SourceObject = Instigator;
		
		GiveAbility(LoadedAbilitySpec);
	}
}

void USoulLikeAbilitySystemComponent::ChangeAbilityInputTag(UKeybindMenuWidgetController* KeybindMenuWidgetController, const FGameplayTag& InputTag)
{
	UAbilityInfo* AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfo(this);
	if(AbilityInfo == nullptr) return;

	if(KeybindMenuWidgetController == nullptr) return;

	const FGameplayTag SelectedAbilityTag = KeybindMenuWidgetController->SelectedAbilityTag;

	if(FGameplayAbilitySpec* PrevAbilitySpec = GetSpecFromInputTag(InputTag))
	{
		const FGameplayTag& PrevAbilityTag = GetAbilityTagFromSpec(*PrevAbilitySpec);
		
		PrevAbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
		
		AbilityInfo->ChangeAbilityInputTag(PrevAbilityTag, FGameplayTag());

		MarkAbilitySpecDirty(*PrevAbilitySpec);
		
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(PrevAbilityTag);
	}
	
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(SelectedAbilityTag))
	{
		AbilitySpec->DynamicAbilityTags.RemoveTag(AbilityInfo->GetAbilityInputTag(SelectedAbilityTag));
		
		AbilityInfo->ChangeAbilityInputTag(SelectedAbilityTag, InputTag);

		AbilitySpec->DynamicAbilityTags.AddTag(InputTag);

		MarkAbilitySpecDirty(*AbilitySpec);
		
		KeybindMenuWidgetController->SelectedAbilityTag = FGameplayTag();
		KeybindMenuWidgetController->OnReceiveInputTagDelegate.Broadcast(SelectedAbilityTag);
	}
}

void USoulLikeAbilitySystemComponent::UpgradeAttributes(const TArray<FAttributeValue>& AttributeValues)
{
	Server_UpgradeAttributes(AttributeValues);
}

void USoulLikeAbilitySystemComponent::Server_UpgradeAttributes_Implementation(
	const TArray<FAttributeValue>& AttributeValues)
{
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage());

	Effect->DurationPolicy = EGameplayEffectDurationType::Instant;

	for(FAttributeValue AttributeValue : AttributeValues)
	{
		FGameplayModifierInfo& ModifierInfo = Effect->Modifiers.Add_GetRef(FGameplayModifierInfo());    
		
		ModifierInfo.Attribute = USoulLikeAttributeSet::TagsToAttributes.FindChecked(AttributeValue.AttributeTag)();
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		ModifierInfo.ModifierMagnitude = FScalableFloat(AttributeValue.AttributeValue);
	}
	
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();

	FGameplayEffectSpec MutableSpec(Effect, EffectContext, 1.f);
	//FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(AttributeEffect, EffectContext, 1.f);
	ApplyGameplayEffectSpecToSelf(MutableSpec);
}

void USoulLikeAbilitySystemComponent::Server_ApplyDamageEffect_Implementation(FDamageEffectParams DamageParams)
{
	USoulLikeAbilitySystemLibrary::ApplyDamageEffect(DamageParams);
}

void USoulLikeAbilitySystemComponent::TryActivateAbilityFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec AbilitySpec = *GetSpecFromAbilityTag(AbilityTag);
	AbilitySpecInputPressed(AbilitySpec);
	if(!AbilitySpec.IsActive())
	{
		TryActivateAbility(AbilitySpec.Handle);
	}
	else
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USoulLikeAbilitySystemComponent::TryActivateAbilityFromInputTag(const FGameplayTag& InputTag)
{
	FGameplayAbilitySpec AbilitySpec = *GetAbilitySpecFromInputTag(InputTag);
	AbilitySpecInputPressed(AbilitySpec);
	if(!AbilitySpec.IsActive())
	{
		TryActivateAbility(AbilitySpec.Handle);
	}
	else
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USoulLikeAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;
	
	FScopedAbilityListLock ActivateScopeLock(*this);

	if(WaitInputDelegate.IsBound())
		WaitInputDelegate.Broadcast(InputTag);
	
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (USoulLikeGameplayAbility* SoulLikeAbility = Cast<USoulLikeGameplayAbility>(AbilitySpec.Ability))
			{
				if (SoulLikeAbility->bIsToggleAbility)
				{
					if (AbilitySpec.IsActive())
					{
						CancelAbilityHandle(AbilitySpec.Handle);
					}
					else
					{
						TryActivateAbility(AbilitySpec.Handle);
					}
					continue;
				}
			}
			AbilitySpecInputPressed(AbilitySpec);
			if(AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void USoulLikeAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;

	FScopedAbilityListLock ActivateScopeLock(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (USoulLikeGameplayAbility* SoulLikeAbility = Cast<USoulLikeGameplayAbility>(AbilitySpec.Ability))
			{
				if (SoulLikeAbility->bIsToggleAbility)
				{
					continue;
				}
			}
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USoulLikeAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;

	FScopedAbilityListLock ActivateScopeLock(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void USoulLikeAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag USoulLikeAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag USoulLikeAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for(FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag USoulLikeAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if(const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* USoulLikeAbilitySystemComponent::GetAbilitySpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActivateScopeLock(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* USoulLikeAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActivateScopeLoc(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()){

		for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags){

			if(Tag.MatchesTag(AbilityTag)){
            
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* USoulLikeAbilitySystemComponent::GetSpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActivateScopeLoc(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()){

		for(FGameplayTag Tag : AbilitySpec.DynamicAbilityTags){

			if(Tag.MatchesTag(InputTag)){
            
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}
