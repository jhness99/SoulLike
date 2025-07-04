// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "InteractionInterface.generated.h"


USTRUCT(BlueprintType)
struct FInteractionTaskInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag InteractionTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString SectionName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bNotEndMontage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString InteractionName = FString();
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULLIKE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetInteractionTag() const;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	// FGameplayTag GetOverlapInteractionTag(UPrimitiveComponent* OverlappedOtherComponent) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FString GetActorName() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetWarpingLocation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator GetWarpingRotation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interaction(AActor* InteractionActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const FInteractionTaskInfo GetInteractionActorInfo() const;
};
