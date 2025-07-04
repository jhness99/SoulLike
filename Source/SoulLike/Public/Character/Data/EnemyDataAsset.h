// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SoulLikeAbilityTypes.h"
#include "EnemyDataAsset.generated.h"

struct FInventoryData;
class ASoulLikeEnemy;
class UBehaviorTree;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAttackMontageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSuperArmor = false;
};

USTRUCT(BlueprintType)
struct FEnemyAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitReactMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> RiposteReactMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttackMontageInfo> AttackMontageInfos;
};

USTRUCT(BlueprintType)
struct FEnemySubBehaviorTree
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SubTreeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> SubBehaviorTree;
};

USTRUCT(BlueprintType)
struct FEnemyMeleeTraceCollisionProperties
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMeleeTraceType MeleeTraceType = EMeleeTraceType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CapsuleHalfHeight = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CapsuleRadius = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EnemyName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASoulLikeEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI Property")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI Property")
	TArray<FEnemySubBehaviorTree> EnemySubBehaviorTrees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mesh Property")
	FVector MeshLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mesh Property")
	FRotator MeshRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Capsule Property")
	float CapsuleHalfHeight = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Capsule Property")
	float CapsuleRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement Property")
	float RunSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement Property")
	float WalkSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute Property")
	float MaxHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute Property")
	float MaxStamina = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute Property")
	float HPBoost = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attribute Property")
	float DamageBoost = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Reward Property")
	float Exp = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Property")
	TArray<FInventoryData> DefaultRightWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat Property")
	TArray<FEnemyMeleeTraceCollisionProperties> MeleeTraceCollisionPropertieses;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim Property")
	FEnemyAnimData EnemyAnimData;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EnemyAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> EnemyDataTable;
};
