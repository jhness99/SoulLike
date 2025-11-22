// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SaveInterface.h"
#include "AutoSaveTestActor.generated.h"

UCLASS()
class SOULLIKE_API AAutoSaveTestActor : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:
	AAutoSaveTestActor();

	/** Save Interface */
	bool GetIsUsedObject_Implementation() const override;
	virtual bool IsDirty() const override;
	virtual void MarkAsDirty() override;
	virtual void MarkAsClean() override;
	
	UPROPERTY(BlueprintReadWrite, Transient, EditAnywhere)
	bool bIsDirty = true;
};
