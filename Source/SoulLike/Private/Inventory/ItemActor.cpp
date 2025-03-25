// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"
#include "Inventory/InventoryItemInstance.h"

#include "Engine/ActorChannel.h"

#include "Net/UnrealNetwork.h"

#include "SoulLikeItemTypes.h"

#include "Interface/CombatInterface.h"


AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetGenerateOverlapEvents(true);
}

bool AItemActor::ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags){

	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	
	return WroteSomething;
}

void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
	
	InitInternal();
}

void AItemActor::SetCollisionEnable(bool bEnable) const
{
	ECollisionEnabled::Type CollisionEnabled = bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
	MeshComponent->SetCollisionEnabled(CollisionEnabled);

	if(!bEnable)
	{
		if(GetOwner()->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_ClearIgnoreActors(GetOwner());
		}
	}
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn = Cast<APawn>(GetOwner());

	if(Pawn->IsLocallyControlled() && MeshComponent)
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
}

void AItemActor::InitInternal()
{
	ItemData = ItemInstance->GetItemData();
	
	UEquipmentData* EquipmentData = Cast<UEquipmentData>(ItemData.Get());
	if(EquipmentData == nullptr) return;

	if(HasAuthority())
	{
		MeshComponent->SetStaticMesh(EquipmentData->ItemMesh);
	}
	else
	{
		MeshComponent->InitializeComponent();
	}
	if(EquipmentData->EquipmentType != EEquipmentType::EET_Weapon) return;
	MeshComponent->SetCollisionProfileName(TEXT("Weapon"));
}

void AItemActor::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;
 	UE_LOG(LogTemp, Warning, TEXT("OnOverlap Actor is %s"), *OtherActor->GetName());

	const FVector TipStart = MeshComponent->GetSocketLocation(FName("TipStart"));
	const FVector TipEnd = MeshComponent->GetSocketLocation(FName("TipEnd"));

	UWeaponData* WeaponData = Cast<UWeaponData>(ItemData);

	const float Radius = WeaponData ? WeaponData->Radius : 5.f;
		
	if(GetOwner()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_MeleeTrace(GetOwner(), TipStart, TipEnd, Radius);
	}
}

void AItemActor::OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance)
{
	if(IsValid(ItemInstance) && !IsValid(OldItemInstance))
	{
		InitInternal();
	}
}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
}
