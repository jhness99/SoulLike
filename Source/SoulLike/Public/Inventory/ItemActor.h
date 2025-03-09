// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

struct FSL_EquipmentData;

UCLASS()
class SOULLIKE_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AItemActor();

	void Init(const FSL_EquipmentData& Data);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;	
};
