// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/InteractionInterface.h"
#include "Interface/SaveInterface.h"
#include "InteractionActor.generated.h"

class UBoxComponent;

UCLASS()
class SOULLIKE_API AInteractionActor : public AActor, public IInteractionInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:
	
	AInteractionActor();

	/**
	 * Interaction Interface 
	 */
	virtual FGameplayTag GetInteractionTag_Implementation() const override { return InteractionTag; }
	virtual FString GetActorName_Implementation() const override { return ActorName; }
	virtual const FInteractionTaskInfo GetInteractionActorInfo_Implementation() const override;
	virtual void Interaction_Implementation(AActor* InteractionActor) override;

	/** Save Interface */
	bool GetIsUsedObject_Implementation() const override;
	virtual bool IsDirty() const override;
	virtual void MarkAsDirty() override;
	virtual void MarkAsClean() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FGameplayTag InteractionTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FString ActorName = FString("");

	//Save Interface
	UPROPERTY(Transient)
	bool bIsDirty = false;
};
