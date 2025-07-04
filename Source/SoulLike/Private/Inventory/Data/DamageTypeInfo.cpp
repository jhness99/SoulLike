// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Data/DamageTypeInfo.h"

FSoulLikeDamageTypeInfo UDamageTypeInfo::FindDamageTypeInfoWithTag(const FGameplayTag& Tag) const
{
	for(const FSoulLikeDamageTypeInfo& Info : DamageTypeInfos)
	{
		if(Tag.MatchesTagExact(Info.DamageTypeTag))
		{
			return Info;
		}
	}
	return FSoulLikeDamageTypeInfo();
}
