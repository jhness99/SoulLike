
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FSoulLikeGameplayTags
{
public:

	static const FSoulLikeGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
private:

	static FSoulLikeGameplayTags GameplayTags;
};