// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SoulLikeAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API ABossAIController : public ASoulLikeAIController
{
	GENERATED_BODY()

public:
	
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	
	
};
