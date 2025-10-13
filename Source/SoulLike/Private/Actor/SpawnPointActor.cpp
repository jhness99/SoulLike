// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SpawnPointActor.h"

#include "Character/SoulLikeEnemy.h"
#include "GameFramework/Character.h"

#include "UI/WidgetController/OverlayWidgetController.h"

#include "Components/BoxComponent.h"

#include "Game/ObjectPoolingSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "SoulLikeFunctionLibrary.h"


ASpawnPointActor::ASpawnPointActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	EnterBossRoom = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterBossRoom"));
	SetRootComponent(Root);
	EnterBossRoom->SetupAttachment(Root);
}

void ASpawnPointActor::BeginPlay()
{
	Super::BeginPlay();

	if(GetGameInstance())
	{
		ObjectPoolingSubsystem = GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>();
		ObjectPoolingSubsystem->OnResetLevelDelegate.AddDynamic(this, &ASpawnPointActor::OnResetLevel);
	}

	if(EnterBossRoom)
	{
		EnterBossRoom->OnComponentBeginOverlap.AddDynamic(this, &ASpawnPointActor::BeginOverlapBossRoomCollision);
		EnterBossRoom->OnComponentEndOverlap.AddDynamic(this, &ASpawnPointActor::EndOverlapBossRoomCollision);
	}
}

void ASpawnPointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority()) return;
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(Player)
	{
		FVector DistanceVector = GetActorLocation() - Player->GetActorLocation();
		float Distance = DistanceVector.Length();

		float MaxDistance = ObjectPoolingSubsystem ? ObjectPoolingSubsystem->MaxMobGenerateDistance : 1000.f;
		
		if(Distance <= MaxDistance)
		{
			SpawnEnemy();
		}
		else
		{
			if(SpawnedEnemy)
			{
				SpawnedEnemy->Disable(false);
				if(SpawnedEnemy->GetOnDeathDelegate().IsBound())
				{
					SpawnedEnemy->GetOnDeathDelegate().RemoveAll(this);
				}
				SpawnedEnemy = nullptr;
			}
		}
	}
}

void ASpawnPointActor::SpawnEnemy()
{
	if(ObjectPoolingSubsystem && SpawnedEnemy == nullptr && !bIsDeadEnemy)
	{
		SpawnedEnemy = ObjectPoolingSubsystem->SpawnEnemy(this, EnemyClass, EnemyId);

		if(SpawnedEnemy && !SpawnedEnemy->GetOnDeathDelegate().IsAlreadyBound(this, &ASpawnPointActor::OnDeathEnemy))
		{
			SpawnedEnemy->GetOnDeathDelegate().AddDynamic(this, &ASpawnPointActor::OnDeathEnemy);
		}
		if(SpawnedEnemy)
		{
			SpawnedEnemy->SetIsBoss(bIsBoss);
		}
	}
}

bool ASpawnPointActor::GetIsUsedObject_Implementation() const
{
	return false;
}

bool ASpawnPointActor::IsDirty() const
{
	return bIsDirty;
}

void ASpawnPointActor::MarkAsDirty()
{
	bIsDirty = true;
}

void ASpawnPointActor::MarkAsClean()
{
	bIsDirty = false;
}

void ASpawnPointActor::OnDeathEnemy()
{
	bIsDeadEnemy = true;
	MarkAsDirty();
}

void ASpawnPointActor::OnResetLevel()
{
	if(bIsRespawn)
		bIsDeadEnemy = false;
	if(SpawnedEnemy)
	{
		if(SpawnedEnemy->GetOnDeathDelegate().IsBound())
		{
			SpawnedEnemy->GetOnDeathDelegate().RemoveAll(this);
		}
		SpawnedEnemy = nullptr;
	}
}

void ASpawnPointActor::BeginOverlapBossRoomCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASpawnPointActor::EndOverlapBossRoomCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bIsBoss)
	{
		if(UOverlayWidgetController* OverlayWidgetController = USoulLikeFunctionLibrary::GetOverlayWidgetController(OtherActor))
		{
			OverlayWidgetController->OnEncounteredBossDelegate.Broadcast(SpawnedEnemy);
		}

		if(SpawnedEnemy)
		{
			SpawnedEnemy->DisableHealthWidget();
		}

		EnterBossRoom->SetCollisionProfileName("BlockAll");
		EnterBossRoom->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	}
}

