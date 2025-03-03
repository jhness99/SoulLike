// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeAssetManager.h"

#include "AbilitySystemGlobals.h"

#include "SoulLikeGameplayTags.h"

USoulLikeAssetManager* USoulLikeAssetManager::Get()
{
	check(GEngine);
	
	USoulLikeAssetManager* SoulLikeAssetManager = Cast<USoulLikeAssetManager>(GEngine->AssetManager);
	return SoulLikeAssetManager;
}

void USoulLikeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FSoulLikeGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
