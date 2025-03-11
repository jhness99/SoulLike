// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"
#include "Inventory/InventoryItemInstance.h"

#include "Engine/ActorChannel.h"

#include "SoulLikeItemTypes.h"

#include "Net/UnrealNetwork.h"


AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetIsReplicated(true);
}

bool AItemActor::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags){

	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);

	return WroteSomething;
}

void AItemActor::Init(UItemData* Data)
{	
	ItemData = Data;
	UEquipmentData* EquipmentData = Cast<UEquipmentData>(Data);
	if(EquipmentData == nullptr) return;
	
	MeshComponent->SetStaticMesh(EquipmentData->ItemMesh);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::InitInternal()
{
	
}

void AItemActor::OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance)
{
	
}


void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
}