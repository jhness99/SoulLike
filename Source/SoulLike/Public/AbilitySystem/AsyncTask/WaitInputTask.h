// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitInputTask.generated.h"

struct FGameplayTag;
class USoulLikeAbilitySystemComponent;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitInputTagSignature, const FGameplayTag&, InputTag);

/**
 * 
 */
UCLASS(BlueprintType)
class SOULLIKE_API UWaitInputTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UWaitInputTask* WaitInputTag(UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UPROPERTY(BlueprintAssignable)
	FWaitInputTagSignature WaitInputTagDelegate;

protected:

	UPROPERTY()
	TObjectPtr<USoulLikeAbilitySystemComponent> ASC;

	FDelegateHandle ASCWaitInputDelegateHandle;
	
	void InputTag(const FGameplayTag& InputTag);
	
};
