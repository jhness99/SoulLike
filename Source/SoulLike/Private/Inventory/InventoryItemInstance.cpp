// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemActor.h"

#include "GameFramework/PlayerState.h"

#include "Interface/CombatInterface.h"

#include "Net/UnrealNetwork.h"

UInventoryItemInstance::UInventoryItemInstance()
{
	
}

void UInventoryItemInstance::Init(const FSL_EquipmentData& Data)
{
	EquipmentData = Data;
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
			ItemActor->Init(EquipmentData);
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
}

UAnimMontage* UInventoryItemInstance::GetMontage()
{
	
	//if(Data)
	return nullptr;
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
	DOREPLIFETIME(UInventoryItemInstance, EquipmentData);
}
