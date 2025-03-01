// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulLikeCharacterBase.generated.h"

UCLASS()
class SOULLIKE_API ASoulLikeCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	ASoulLikeCharacterBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
