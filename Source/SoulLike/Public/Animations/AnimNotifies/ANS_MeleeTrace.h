// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SoulLikeAbilityTypes.h"
#include "ANS_MeleeTrace.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UANS_MeleeTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	UANS_MeleeTrace();

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeleeTraceType MeleeTraceType = EMeleeTraceType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityIndex;
	
private:

	void SetWeaponCollisionEnable(USkeletalMeshComponent* MeshComp, bool bEnable);

	UPROPERTY()
	TObjectPtr<AActor> Owner = nullptr; 
};
