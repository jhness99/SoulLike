// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"
#include "Inventory/InventoryItemInstance.h"

#include "Inventory/Data/EquipmentData.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetIsReplicated(true);
}

void AItemActor::Init(const FSL_EquipmentData& Data)
{
	MeshComponent->SetStaticMesh(Data.ItemMesh);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}
