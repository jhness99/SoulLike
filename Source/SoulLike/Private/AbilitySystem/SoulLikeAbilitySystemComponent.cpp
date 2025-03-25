// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/SoulLikeGameplayAbility.h"

void USoulLikeAbilitySystemComponent::GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, APawn* Instigator)
{
	for(const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if(const USoulLikeGameplayAbility* SoulLikeAbility = Cast<USoulLikeGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(SoulLikeAbility->StartupInputTag);
			AbilitySpec.SourceObject = Instigator;
			GiveAbility(AbilitySpec);
		}
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

FGameplayAbilitySpec USoulLikeAbilitySystemComponent::GetAbilitySpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActivateScopeLock(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return AbilitySpec;
		}
	}
	return FGameplayAbilitySpec();
}
