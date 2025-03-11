// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemActor.h"
#include "Inventory/Data/ItemDataAsset.h"

#include "Game/SoulLikeGameInstance.h"

#include "GameFramework/PlayerState.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeItemTypes.h"

#include "Net/UnrealNetwork.h"

UInventoryItemInstance::UInventoryItemInstance()
{
	
}

void UInventoryItemInstance::Init(const FInventoryData& Data)
{
	InventoryData = Data;
}

void UInventoryItemInstance::OnEquip(AActor* Owner)
{
	if(const APlayerState* PS = Cast<APlayerState>(Owner)) Owner = PS->GetPawn();
	
	if(Owner->Implements<UCombatInterface>())
	{
		FTransform Transform;
		Transform.SetLocation(Owner->GetActorLocation());

		if(UWorld* World = Owner->GetWorld())
		{
			ItemActor = World->SpawnActorDeferred<AItemActor>(AItemActor::StaticClass(), Transform);
			SetupItemData();
			ItemActor->Init(ItemData);
			ItemActor->FinishSpawning(Transform);

			ICombatInterface::Execute_EquipOnCharacter(Owner, ItemActor);
		}
	}
}

void UInventoryItemInstance::OnUnEquip()
{
	if(ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
	if(ItemData)
	{
		ItemData->MarkAsGarbage();
		ItemData = nullptr;
	}
}

UAnimMontage* UInventoryItemInstance::GetMontage() const
{
	if(UWeaponData* WeaponData = Cast<UWeaponData>(ItemData))
	{
		return WeaponData->AttackMontage;
	}
	
	return nullptr;
}

void UInventoryItemInstance::SetupItemData()
{
	if(GetWorld())
	{
		if(USoulLikeGameInstance* GameInstance = Cast<USoulLikeGameInstance>(GetWorld()->GetGameInstance()))
		{
			ItemData = GameInstance->ItemDataAsset->FindItemDataFromIndexAndItemType(InventoryData.ItemType, InventoryData.ItemID);
		}
	}
}

void UInventoryItemInstance::OnRep_InventoryData(FInventoryData OldInventoryData)
{
	SetupItemData();
}


void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
	DOREPLIFETIME(UInventoryItemInstance, InventoryData);
}
