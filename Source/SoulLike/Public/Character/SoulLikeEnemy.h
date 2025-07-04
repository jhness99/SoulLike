// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SoulLikeCharacterBase.h"
#include "Interface/ObjectPoolingInterface.h"
#include "Character/Data/EnemyDataAsset.h"
#include "SoulLikeEnemy.generated.h"

class UBehaviorTree;
class ASoulLikeAIController;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class SOULLIKE_API ASoulLikeEnemy : public ASoulLikeCharacterBase, public IObjectPoolingInterface
{
	GENERATED_BODY()

public:
	
	ASoulLikeEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void Init(AActor* InSpanwerActor, FEnemyData Data) override;
	virtual void Disable(bool bKilled) override;

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetRiposteReactMontage_Implementation() override;
	virtual void SetWeaponCollisionEnable_Implementation(bool bEnable, EMeleeTraceType MeleeTraceType, FName InAbilityIndex);

	UFUNCTION(BlueprintPure)
	bool SwitchWalkSpeedWithStatusTag(const FGameplayTag& StatusTag) const;
	
	void DisableEffect();

	virtual void DeathToCharacter(AActor* InstigatorActor) override;
	
	FAttackMontageInfo GetRandomAttackMontage() const;
	UFUNCTION(BlueprintPure)
	const FEnemyData& GetEnemyData() const;

	void SetIsBoss(bool InIsBoss){ bIsBoss = InIsBoss; }

	void DisableHealthWidget() const;

protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	virtual void SetupDamageParams(FDamageEffectParams& DamageEffectParams, float DamageBoost) override;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	// TObjectPtr<UWidgetComponent> HealthWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<ASoulLikeAIController> SoulLikeAIController;

private:

	const FString GetTraceTypeString(EMeleeTraceType MeleeTraceType);

	UFUNCTION()
	void OnOverlapMeleeTraceCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetActorEnableCollision(bool EnableCollision);
	void SetupActorWithEnmeyData();
	
	UFUNCTION()
	void OnRep_EnemyData(FEnemyData OldEnemyData);
	
	UPROPERTY(Replicated)
	TObjectPtr<AActor> SpawnerActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	TMap<EMeleeTraceType, TObjectPtr<UCapsuleComponent>> TraceCollisions;
	
	// UPROPERTY()
	// TObjectPtr<UCapsuleComponent> RightMeleeTraceCollision;
	//
	// UPROPERTY()
	// TObjectPtr<UCapsuleComponent> LeftMeleeTraceCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyID", meta=(AllowPrivateAccess=true))
	FName EnemyId;
	
	UPROPERTY(ReplicatedUsing=OnRep_EnemyData)
	FEnemyData EnemyData;

	
	FTimerHandle DisableTimer;
	bool bIsBoss = false;
	
};