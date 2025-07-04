// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "SoulLikeAIController.generated.h"

class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASoulLikeAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	UBehaviorTreeComponent* GetBehaviorComponent() const;
	
	//IGenericTeamAgentInterface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
	
	static ETeamAttitude::Type TeamAttitudeSolver(FGenericTeamId A, FGenericTeamId B);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTagContainer DisableTargetFocusTags;

protected:

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
private:

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
