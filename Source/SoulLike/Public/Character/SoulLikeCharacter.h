// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SoulLikeCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "SoulLikeCharacter.generated.h"

class IInteractionInterface;
class UItemDataAsset;
class USpringArmComponent;
class UCameraComponent;
class USoulLikeSaveGame;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeCharacter : public ASoulLikeCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	ASoulLikeCharacter();
	
	//Combat Interface
	virtual void SetWarpingLocationAndRotation(FVector Location, FRotator Rotation) override;
	virtual void SetWarpingLocation() override;
	virtual void SetWarpingRotation(FRotator TargetRotation = FRotator::ZeroRotator) override;
	
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetRiposteReactMontage_Implementation() override;
	
	UCameraComponent* GetPlayerCameraComponent() const;
	float GetMouseXInput() const;

	UFUNCTION(BlueprintCallable)
	void Pickup(FInventoryData InventoryData);
	
	void LoadProgress(USoulLikeSaveGame* SaveGame = nullptr);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> ItemDataAsset;

protected:

	virtual void InitAbilityActorInfo() override;

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void DisableCharacter_Implementation() override;
	
	/** Player Interface */
	virtual void RefreshInventory_Implementation() override;
	virtual void TriggerSavePointMenuWidget_Implementation() override;
	virtual void SaveProgress_Implementation() const override;
	virtual void GiveExp_Implementation(int32 ExpValue) override;

	/** Interaction Interface*/
	virtual void Interaction_Implementation(AActor* InteractionActor) override;
	virtual FGameplayTag GetInteractionTag_Implementation() const override;
	virtual const FInteractionTaskInfo GetInteractionActorInfo_Implementation() const override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SetWarpingTargetFromInteractionActor(bool bHasWarpingPoint = false);

private:

	bool bAbilityActorInfoInitialized = false;

	UFUNCTION()
	void Rebirth();
    
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> InteractionActors;
	
	UPROPERTY()
	TObjectPtr<AActor> SelectedInteractionActor;

	UPROPERTY()
	FTransform RespawnPoint;

	FTimerHandle RebirthTimerHandle;
	
};