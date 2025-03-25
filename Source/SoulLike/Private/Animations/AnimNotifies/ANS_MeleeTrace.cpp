// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/ANS_MeleeTrace.h"

#include "GameFramework/Character.h"

#include "Inventory/InventoryItemInstance.h"

#include "Interface/CombatInterface.h"

UANS_MeleeTrace::UANS_MeleeTrace()
{
	bShouldFireInEditor = false;
}

void UANS_MeleeTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Owner = MeshComp->GetOwner();
	SetWeaponCollisionEnable(MeshComp, true);
}

void UANS_MeleeTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	SetWeaponCollisionEnable(MeshComp, false);
}

void UANS_MeleeTrace::SetWeaponCollisionEnable(USkeletalMeshComponent* MeshComp, bool bEnable)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if(Character == nullptr) return;

	if(Character->Implements<UCombatInterface>())
	{
		UInventoryItemInstance* WeaponInstance = ICombatInterface::Execute_GetCurrentWeapon(Character);
		if(WeaponInstance == nullptr) return;

		WeaponInstance->SetCollisionEnable(bEnable);
	}
}

