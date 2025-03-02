// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SoulLikeCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SOULLIKE_API ASoulLikeCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ASoulLikeCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
public:	
	
};
