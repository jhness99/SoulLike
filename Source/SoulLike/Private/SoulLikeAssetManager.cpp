// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeAssetManager.h"

#include "AbilitySystemGlobals.h"

USoulLikeAssetManager* USoulLikeAssetManager::Get()
{
	check(GEngine);
	
	USoulLikeAssetManager* SoulLikeAssetManager = Cast<USoulLikeAssetManager>(GEngine->AssetManager);
	return SoulLikeAssetManager;
}

void USoulLikeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
