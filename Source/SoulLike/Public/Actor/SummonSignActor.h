// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractionActor.h"
#include "SummonSignActor.generated.h"

USTRUCT(BlueprintType)
struct FSummonSignState
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString SteamUID = FString("MySign");

	UPROPERTY(BlueprintReadOnly)
	FString ProfileName = FString("NoName");

	UPROPERTY(BlueprintReadOnly)
	int32 Level = 1;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASummonSignActor : public AInteractionActor
{
	GENERATED_BODY()

public:

	ASummonSignActor();
	
	void Init(const FSummonSignState& InSummonSignState);

	virtual void Interaction_Implementation(AActor* InteractionActor) override;

	UFUNCTION()
	void OnClickedConfirmButton();

	UFUNCTION()
	void OnClickedCancelButton();

	UFUNCTION(BlueprintPure)
	FORCEINLINE FSummonSignState GetSummonSignState() { return SummonSignState; }

private:

	UPROPERTY()
	FSummonSignState SummonSignState;
};
