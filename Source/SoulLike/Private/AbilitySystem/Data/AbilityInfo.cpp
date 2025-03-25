// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FSoulLikeAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const FSoulLikeAbilityInfo& Info : AbilityInformation){

		if(Info.AbilityTag == AbilityTag){
        
			return Info;
		}
	}

	if(bLogNotFound){

		UE_LOG(LogTemp, Error, TEXT("Can't Find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FSoulLikeAbilityInfo();
}
