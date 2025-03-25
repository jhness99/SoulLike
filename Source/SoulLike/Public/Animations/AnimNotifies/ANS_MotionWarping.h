// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "ANS_MotionWarping.generated.h"

UENUM(BlueprintType)
enum class EAnimWarpingType : uint8
{
	EWT_LocationAndRotation UMETA(DisplayName = "LocationAndRotation"),
	EWT_Location UMETA(DisplayName = "Location"),
	EWT_Rotation UMETA(DisplayName = "Rotation")
};

USTRUCT(BlueprintType)
struct FMotionWarpingParams
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAnimWarpingType WarpingType = EAnimWarpingType::EWT_LocationAndRotation;;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUpdateTick = false;
};

/**
 * 
 */
UCLASS()
class SOULLIKE_API UANS_MotionWarping : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()
	
	UANS_MotionWarping(const FObjectInitializer& ObjectInitializer);

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	
protected:

	void SetMotionWarpingTarget() const;
	
	UPROPERTY(EditAnywhere)
	FMotionWarpingParams MotionWarpingParams;
	
	UPROPERTY()
	TObjectPtr<AActor> Owner = nullptr; 
};
