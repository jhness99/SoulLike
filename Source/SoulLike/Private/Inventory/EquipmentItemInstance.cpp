// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EquipmentItemInstance.h"

#include "SoulLikeGameplayTags.h"
#include "Inventory/ItemActor.h"

#include "Interface/CombatInterface.h"

#include "GameFramework/PlayerState.h"

#include "Net/UnrealNetwork.h"


UAnimMontage* UEquipmentItemInstance::GetAttackMontage() const
{
	if(UWeaponData* WeaponData = GetWeaponData())
	{
		return WeaponData->AttackMontage;
	}
	
	return nullptr;
}

UAnimMontage* UEquipmentItemInstance::GetRiposteMontage() const
{
	if(UWeaponData* WeaponData = GetWeaponData())
	{
		return WeaponData->RiposteMontage;
	}
	
	return nullptr;
}

void UEquipmentItemInstance::OnEquip(AActor* Owner, const FGameplayTag& SlotTag)
{
	if(const APlayerState* PS = Cast<APlayerState>(Owner)) Owner = PS->GetPawn();
	if(Owner == nullptr) return;
	if(Owner->Implements<UCombatInterface>())
	{
		FTransform Transform;
		Transform.SetLocation(Owner->GetActorLocation());

		if(UWorld* World = Owner->GetWorld())
		{
			ItemActor = World->SpawnActorDeferred<AItemActor>(AItemActor::StaticClass(), Transform, Owner);
			SetupItemData(Owner);
			ItemActor->Init(this);
			ItemActor->FinishSpawning(Transform);

			FName SocketName = FName(TEXT("RightWeaponSocket"));
			if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_RightWeapon))
			{
				SocketName = FName(TEXT("RightWeaponSocket"));
			}
			else if(SlotTag.MatchesTagExact(FSoulLikeGameplayTags::Get().EquipSlot_LeftWeapon))
			{
				SocketName = FName(TEXT("LeftWeaponSocket"));
			}

			ICombatInterface::Execute_EquipOnCharacter(Owner, ItemActor, SocketName);
		}
	}
}

void UEquipmentItemInstance::OnUnEquip()
{
	if(ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
}

void UEquipmentItemInstance::SetCollisionEnable(bool bEnable) const
{
	if(ItemActor)
	{
		ItemActor->SetCollisionEnable(bEnable);
	}
}

void UEquipmentItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UEquipmentItemInstance, ItemActor);
}
