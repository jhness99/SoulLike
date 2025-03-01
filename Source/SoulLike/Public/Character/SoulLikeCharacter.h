// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SoulLikeCharacterBase.h"
#include "SoulLikeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeCharacter : public ASoulLikeCharacterBase
{
	GENERATED_BODY()
	
public:

	ASoulLikeCharacter();

protected:


private:
    
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;
};
