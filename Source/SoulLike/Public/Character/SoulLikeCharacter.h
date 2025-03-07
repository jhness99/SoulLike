// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SoulLikeCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "SoulLikeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeCharacter : public ASoulLikeCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	ASoulLikeCharacter();

protected:

	virtual void InitAbilityActorInfo() override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void TryActiveAbilityWithInputTag_Implementation(const FGameplayTag& InputTag) override;

private:
    
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;
};
