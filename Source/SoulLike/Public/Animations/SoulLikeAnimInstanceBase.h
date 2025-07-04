// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "SoulLikeAnimInstanceBase.generated.h"

class UCharacterMovementComponent;
class UAbilitySystemComponent;
class ASoulLikeCharacterBase;
struct FPoseSearchQueryTrajectory;

USTRUCT(BlueprintType)
struct FCharacterBaseAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FCharacterBaseAnimInstanceProxy() : FAnimInstanceProxy() {};

	FCharacterBaseAnimInstanceProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance)
	{
		
	}

	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	
	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "Pawn")
	TWeakObjectPtr<ASoulLikeCharacterBase> Pawn = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "Pawn")
	TWeakObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovement = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	FGameplayTagContainer HasGameplayTags;

	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	FGameplayTagContainer CurrentWeaponItemTypes;

	UPROPERTY(Transient, BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	bool bIsTargetLock = false;
};

/**
 * Animation Thread와 GameThread
 *
 * AnimInstanceProxy를 사용하는 이유는 병렬로 Thread를 나눠서 멀티스레드로 애니메이션의 포즈 계산을 하기 위함이다.
 * 이때 GameThread 도중에 AnimationThread가 Fork 되어서 GameThread의 실제 UObjcet를 가지게 되고,
 * AnimationThread에서 해당하는 값을 가지고 평가하게 된다. 물론 평가하기 위한 변수또한 이 때 계산하게 된다.
 */
UCLASS()
class SOULLIKE_API USoulLikeAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	void SetMirror(bool InbMirror) { bMirror = InbMirror; }
	void SetIdleMirror(bool IsbIdleMirror) { bIdleMirror = IsbIdleMirror; }

	bool GetMirror() const {return bMirror;}
	bool GetIdleMirror() const {return bIdleMirror; }
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<ASoulLikeCharacterBase> Pawn = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGameplayTagContainer HasGameplayTags;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGameplayTagContainer CurrentWeaponItemTypes;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bMirror = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIdleMirror = false;
	
private:
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return new FCharacterBaseAnimInstanceProxy(this);
	}
 
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override
	{
		delete static_cast<FCharacterBaseAnimInstanceProxy*>(InProxy);
	}
 
	friend struct FCharacterBaseAnimInstanceProxy;
};
