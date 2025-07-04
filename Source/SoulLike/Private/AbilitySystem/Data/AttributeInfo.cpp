// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FSoulLikeAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag) const
{
	for(const FSoulLikeAttributeInfo& Info : AttributeInformation)
	{
		if(Info.AttributeTag == AttributeTag){
        
			if(Info.AttributeTag.MatchesTagExact(AttributeTag))
			{
				return Info;
			}
		}
	}

	return FSoulLikeAttributeInfo();
}

int32 UAttributeInfo::GetRequireExp(int32 Level) const
{
	return RequireEXP.GetValueAtLevel(Level);
}
