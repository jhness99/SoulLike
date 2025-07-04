// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"

ABossAIController::ABossAIController()
{
	PerceptionComponent->SetDominantSense(DamageConfig->GetSenseImplementation());
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
}
