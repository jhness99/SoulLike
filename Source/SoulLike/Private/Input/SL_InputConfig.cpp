// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SL_InputConfig.h"

const UInputAction* USL_InputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for(const FSL_InputAction& InputAction : InputActions)
	{
		if(InputAction.InputAction && InputAction.InputTag == InputTag)
		{
			return InputAction.InputAction;
		}
	}
	return nullptr;
}

FGameplayTag USL_InputConfig::FindInputTagForAbilityTags(const FGameplayTagContainer& AbilityTags) const
{
	for(const FAbilityKeyBind& AbilityKeyBind : AbilityKeyBinds)
	{
		if(AbilityTags.HasTagExact(AbilityKeyBind.AbilityTag))
		{
			return AbilityKeyBind.InputTag;
		}
	}
	return FGameplayTag();
}
