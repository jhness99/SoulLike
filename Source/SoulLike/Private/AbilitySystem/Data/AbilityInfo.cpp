// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FSoulLikeAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const FSoulLikeAbilityInfo& Info : AbilityInformation)
	{
		if(Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FSoulLikeAbilityInfo();
}

FInteractionTaskInfo UAbilityInfo::FindInteractionTaskInfoForTag(const FGameplayTag& InteractionTag,
	bool bLogNotFound) const
{
	for(const FInteractionTaskInfo& Info : InteractionTaskInfomation)
	{
		if(InteractionTag.MatchesTag(Info.InteractionTag))
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find info for AbilityTag [%s] on AbilityInfo [%s]"), *InteractionTag.ToString(), *GetNameSafe(this));
	}
	return FInteractionTaskInfo();
}

bool UAbilityInfo::ChangeAbilityInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	for(FSoulLikeAbilityInfo& Info : AbilityInformation)
	{
		if(Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			Info.InputTag = InputTag;
			return true;
		}
	}
	return false;
}

FGameplayTag UAbilityInfo::GetAbilityInputTag(const FGameplayTag& Ability)
{
	return FindAbilityInfoForTag(Ability).InputTag;
}
