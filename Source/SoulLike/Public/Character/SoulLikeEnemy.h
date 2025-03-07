// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SoulLikeCharacterBase.h"
#include "SoulLikeEnemy.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeEnemy : public ASoulLikeCharacterBase
{
	GENERATED_BODY()

public:
	
	ASoulLikeEnemy();

protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;
};
