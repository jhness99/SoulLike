// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"
#include "Inventory/InventoryItemInstance.h"

#include "Engine/ActorChannel.h"

#include "Net/UnrealNetwork.h"

#include "SoulLikeItemTypes.h"
#include "Character/SoulLikeCharacterBase.h"

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

}

void AItemActor::DetachAndDestroy()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));

	// 충돌 비활성화 (다른 액터와 충돌 안 하게)
	MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(true);

	// 3초후 삭제
	if(HasAuthority())
		SetLifeSpan(3.f);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn = Cast<APawn>(GetOwner());
	if(Pawn == nullptr) return;
	if(Pawn->IsLocallyControlled() || Pawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		if(MeshComponent)
		{
			MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
		}
	}
}

void AItemActor::InitInternal()
{
	ItemData = ItemInstance->GetItemData();
	
	UEquipmentData* EquipmentData = Cast<UEquipmentData>(ItemData.Get());
	if(EquipmentData == nullptr)
	{
		MeshComponent->SetVisibility(false);
		MeshComponent->SetHiddenInGame(true);
		MeshComponent->SetComponentTickEnabled(false);
		MeshComponent->Deactivate();
		return;
	}

	if(HasAuthority())
	{
		MeshComponent->SetStaticMesh(EquipmentData->ItemMesh);
	}
	else
	{
		MeshComponent->InitializeComponent();
	}

	MeshComponent->SetCollisionProfileName(TEXT("Weapon"));
}

void AItemActor::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;
 	
	// 서버에서 검증하지 않는 단순한 방법
	// const FVector TipStart = MeshComponent->GetSocketLocation(FName("TipStart"));
	// const FVector TipEnd = MeshComponent->GetSocketLocation(FName("TipEnd"));

	// 클라이언트에서의 무기와 Owner와의 상대적인 Transform을 보내서 서버에서 캐릭터의 위치를 기반으로 socket의 Transform을 다시계산해서 검증
	FTransform TipStartTransform = MeshComponent->GetSocketTransform("TipStart", RTS_World);
	FTransform TipEndTransform = MeshComponent->GetSocketTransform("TipEnd", RTS_World);
	FTransform ParentWorldTransform = GetOwner()->GetActorTransform();  // AActor2는 AttachParent
	FTransform TipStartRelativeToParent = TipStartTransform.GetRelativeTransform(ParentWorldTransform);
	FTransform TipEndRelativeToParent = TipEndTransform.GetRelativeTransform(ParentWorldTransform);
	
	UWeaponData* WeaponData = Cast<UWeaponData>(ItemData);

	const float Radius = WeaponData ? WeaponData->Radius : 5.f;
		
	if(GetOwner()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_MeleeTrace(GetOwner(), TipStartRelativeToParent, TipEndRelativeToParent, Radius);
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
