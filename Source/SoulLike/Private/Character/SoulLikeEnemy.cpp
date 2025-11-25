// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeEnemy.h"

#include "SoulLikeFunctionLibrary.h"
#include "Character/Data/EnemyDataAsset.h"

#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "AI/SoulLikeAIController.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SoulLikeCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/EquipmentItemInstance.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemActor.h"

#include "Net/UnrealNetwork.h"


ASoulLikeEnemy::ASoulLikeEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulLikeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USoulLikeAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);	

	SetReplicates(true);
	
	bAlwaysRelevant = true;
}

void ASoulLikeEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
	if(SoulLikeAIController = Cast<ASoulLikeAIController>(NewController))
	{
		if(SoulLikeAIController->GetBlackboardComponent())
		{
			SoulLikeAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

			if(SpawnerActor)
			{
				SoulLikeAIController->GetBlackboardComponent()->SetValueAsVector(FName("LocationBeforeChasingTarget"), SpawnerActor->GetActorLocation());
				SoulLikeAIController->GetBlackboardComponent()->SetValueAsRotator(FName("RotationBeforeChasingTarget"), SpawnerActor->GetActorRotation());
			}
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				for(const FGameplayTag& Tag : SoulLikeAIController->DisableTargetFocusTags)
				{
					if(ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).IsBoundToObject(this)) continue;
					
					ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddLambda([this, ASC](const FGameplayTag Tag, int32 NewCount)
					{
						if(SoulLikeAIController && ASC && SoulLikeAIController->GetBlackboardComponent())
						{
							if(ASC->HasAnyMatchingGameplayTags(SoulLikeAIController->DisableTargetFocusTags))
							{
								SoulLikeAIController->GetBlackboardComponent()->SetValueAsBool(FName("DisableTargetFocus"), true);
							}
							else
							{
								SoulLikeAIController->GetBlackboardComponent()->SetValueAsBool(FName("DisableTargetFocus"), false);
							}
						}
					});
				}
			}
		}
		
		SoulLikeAIController->RunBehaviorTree(BehaviorTree);

		if(SoulLikeAIController->GetBehaviorComponent())
		{	
			for(const FEnemySubBehaviorTree& EnemySubBehaviorTree : EnemyData.EnemySubBehaviorTrees)
			{
				SoulLikeAIController->GetBehaviorComponent()->SetDynamicSubtree(EnemySubBehaviorTree.SubTreeTag, EnemySubBehaviorTree.SubBehaviorTree);
			}
		}
	}

	if(InventoryComponent)
	{
		InventoryComponent->SetDefaultWeapons(EnemyData.DefaultRightWeapons);
		InventoryComponent->Init();
		InventoryComponent->SetupDefaultInventoryList();
		InventoryComponent->EquipItem();
	}
}

void ASoulLikeEnemy::Init(AActor* InSpawnerActor, FEnemyData Data)
{
	EnemyData = Data;
	SpawnerActor = InSpawnerActor;
	SetupActorWithEnmeyData();
	if(HasAuthority())
	{
		InitializeDefaultAttributes();
		GetAbilitySystemComponent()->CancelAllAbilities();
		ApplyEffectToSelf(FullHealthEffectClass, 1.f);
	}
}

void ASoulLikeEnemy::Disable(bool bKilled)
{
	SetActorTickEnabled(false);
	
	if(GetMesh() && bIsBoss)
	{
		GetMesh()->SetSimulatePhysics(false);
	}

	if(GetCurrentWeapon_Implementation())
	{
		if(AItemActor* WeaponActor = GetCurrentWeapon_Implementation()->GetItemActor())
		{
			WeaponActor->DetachAndDestroy();
		}
	}

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->StopMovement();
		AI->UnPossess();
		AI->Destroy();
	}
	
	if(!bKilled)
	{
		EnemyData = FEnemyData();
		SetupActorWithEnmeyData();
		OnDisabledObjectDelegate.Broadcast(this);
	}

	for(const TTuple<EMeleeTraceType, TObjectPtr<UCapsuleComponent>>& Pair : TraceCollisions)
	{
		Pair.Value->DestroyComponent();
		Pair.Value->MarkAsGarbage();
	}
	TraceCollisions.Empty();

	if(InventoryComponent)
	{
		InventoryComponent->Reset();
	}

	if(TargetLockMarkWidgetComponent)
	{
		TargetLockMarkWidgetComponent->Deactivate();
	}

	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	TraceCollisions.Reset();
	OnChangedVisible.Broadcast(false, 0.f);

	if(bIsBoss && bKilled)
	{
		if(GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(DisableTimer, this, &ASoulLikeEnemy::DisableEffect, 3.f, false);
		}
	}
}

UAnimMontage* ASoulLikeEnemy::GetHitReactMontage_Implementation()
{
	return EnemyData.EnemyAnimData.HitReactMontage;
}

UAnimMontage* ASoulLikeEnemy::GetRiposteReactMontage_Implementation()
{
	return EnemyData.EnemyAnimData.RiposteReactMontage;
}

void ASoulLikeEnemy::SetWeaponCollisionEnable_Implementation(bool bEnable, EMeleeTraceType MeleeTraceType, FName InAbilityIndex)
{
	if(InventoryComponent && InventoryComponent->GetCurrentWeapon())
	{
		if(InventoryComponent->GetCurrentWeapon()->GetWeaponData()->ItemMesh != nullptr)
		{
			InventoryComponent->GetCurrentWeapon()->SetCollisionEnable(bEnable);
		}
	}
	else
	{
		ECollisionEnabled::Type CollisionEnabled = bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
		if(TraceCollisions.Num() > 0)
		{
			if(UCapsuleComponent* TraceCollision = *TraceCollisions.Find(MeleeTraceType))
				TraceCollision->SetCollisionEnabled(CollisionEnabled);
		}
		AbilityIndex = InAbilityIndex;
	}
	
	if(!bEnable)
	{
		ClearIgnoreActors_Implementation();
		AbilityIndex = FName("");
	}
}

bool ASoulLikeEnemy::SwitchWalkSpeedWithStatusTag(const FGameplayTag& StatusTag) const
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();

	if(StatusTag.MatchesTagExact(GameplayTags.Status_AI_Idle) || StatusTag.MatchesTagExact(GameplayTags.Status_AI_Battle))
	{
		SwitchWalkSpeed_Implementation();
		return true;
	}
	if(StatusTag.MatchesTagExact(GameplayTags.Status_AI_Chasing))
	{
		SwitchRunSpeed_Implementation();
		return true;
	}
	return false;
}

void ASoulLikeEnemy::DeathToCharacter(AActor* InstigatorActor)
{
	Super::DeathToCharacter(InstigatorActor);

	if(InstigatorActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_GiveExp(InstigatorActor, EnemyData.Exp);
	}
	
	USoulLikeAbilitySystemLibrary::SetAIStatusWithTag(this, FSoulLikeGameplayTags::Get().Status_AI_Death);
	
	Disable(true);
}

FAttackMontageInfo ASoulLikeEnemy::GetRandomAttackMontage() const
{
	TArray<FAttackMontageInfo> AttackMontageInfos = EnemyData.EnemyAnimData.AttackMontageInfos;
	
	int32 MontageNum = AttackMontageInfos.Num();
	
	if(MontageNum == 0) return FAttackMontageInfo();

	int32 RandIndex = FMath::RandRange(0, MontageNum - 1);
	return AttackMontageInfos[RandIndex];
}

const FEnemyData& ASoulLikeEnemy::GetEnemyData() const
{
	return EnemyData;
}

void ASoulLikeEnemy::DisableHealthWidget() const
{
	if(HealthWidgetComponent)
	{
		HealthWidgetComponent->Deactivate();
	}
}

void ASoulLikeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	InitAbilityActorInfo();

	GiveAbilities();
}

void ASoulLikeEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if(HasAuthority()) InitializeDefaultAttributes();
}

void ASoulLikeEnemy::InitializeDefaultAttributes() const
{
	USoulLikeAbilitySystemLibrary::InitializeDefaultAttributes_Enemy(this, AbilitySystemComponent, EnemyData);
}

void ASoulLikeEnemy::SetupDamageParams(FDamageEffectParams& DamageEffectParams, float DamageBoost)
{
	if(UEquipmentItemInstance* CurrentWeapon = GetCurrentWeapon_Implementation())
	{
		if(UWeaponData* CurrentWeaponData = CurrentWeapon->GetWeaponData())
		{
			Super::SetupDamageParams(DamageEffectParams, EnemyData.DamageBoost);
		}
	}
	else
	{
		if(UAbilityInfo* AbilityInfo = USoulLikeFunctionLibrary::GetAbilityInfo(this))
		{
			if(AbilityInfo->EnemyAbilityDamageInfo)
			{
				FEnemyAbilityDamageInfo* AbilityDamageInfo = AbilityInfo->EnemyAbilityDamageInfo->FindRow<FEnemyAbilityDamageInfo>(AbilityIndex, FString("Not Found"));
				if(AbilityDamageInfo)
				{
					DamageEffectParams.DamageInfos = AbilityDamageInfo->DamageInfos;
				}
				for(FWeaponDamageInfo& DamageInfo : DamageEffectParams.DamageInfos)
				{
					DamageInfo.Damage *= EnemyData.DamageBoost;
				}
			}
		}
	}
}

void ASoulLikeEnemy::Multicast_SetActorEnableCollision_Implementation(bool EnableCollision)
{
	SetActorEnableCollision(EnableCollision);
}

const FString ASoulLikeEnemy::GetTraceTypeString(EMeleeTraceType MeleeTraceType)
{
	FString TypeString = FString("");
	const UEnum* EnumPtr = StaticEnum<EMeleeTraceType>();
	if (EnumPtr)
	{
		TypeString = EnumPtr->GetNameStringByValue(static_cast<int64>(MeleeTraceType));
	}
	
	return TypeString;
}

void ASoulLikeEnemy::DisableEffect()
{
	if(GetMesh())
	{
		GetMesh()->SetSkeletalMesh(nullptr);
	}
}

void ASoulLikeEnemy::OnOverlapMeleeTraceCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float Radius = 5.f;
	FString TraceTypeString = FString("");
	if(UCapsuleComponent* OverlapCollision = Cast<UCapsuleComponent>(OverlappedComponent))
	{
		EMeleeTraceType TraceType = *TraceCollisions.FindKey(OverlapCollision);
		TraceTypeString = GetTraceTypeString(TraceType);

		Radius = OverlapCollision->GetScaledCapsuleRadius();
	}
	const FName TipStart = FName(FString::Printf(TEXT("%sTipStart"), *TraceTypeString));
	const FName TipEnd = FName(FString::Printf(TEXT("%sTipEnd"), *TraceTypeString));
	
	FTransform TipStartTransform = GetMesh()->GetSocketTransform(TipStart);
	FTransform TipEndTransform = GetMesh()->GetSocketTransform(TipEnd);
	FTransform ParentWorldTransform = GetActorTransform();  // AActor2는 AttachParent
	FTransform TipStartRelativeToParent = TipStartTransform.GetRelativeTransform(ParentWorldTransform);
	FTransform TipEndRelativeToParent = TipEndTransform.GetRelativeTransform(ParentWorldTransform);
	
	MeleeTrace_Implementation(TipStartRelativeToParent, TipEndRelativeToParent, Radius);
}

void ASoulLikeEnemy::SetupActorWithEnmeyData()
{
	SetActorHiddenInGame(false);
	Multicast_SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	bSimGravityDisabled = true;
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	if(GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(EnemyData.AnimClass);
		GetMesh()->SetSkeletalMesh(EnemyData.SkeletalMesh);
		if(HasAuthority())
		{
			GetMesh()->SetRelativeLocation(EnemyData.MeshLocation);
			GetMesh()->SetRelativeRotation(EnemyData.MeshRotation);
		}else
		{
			//해결한 문제
			//릭터 유형의 액터의 경우 메시 구성 요소의 위치는 게임 플레이 시작 시 캐시됩니다.
			//서버가 클라이언트에 위치 업데이트를 보낼 때 이 캐시된 위치가 보간 대상 위치로 사용됩니다.
			//하지만 안타깝게도 이 캐시된 위치는 메시 구성 요소의 위치에 적용된 동적 런타임 수정 사항을 완전히 무시합니다.
			CacheInitialMeshOffset(EnemyData.MeshLocation, EnemyData.MeshRotation);
		}
	}
	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(EnemyData.CapsuleHalfHeight);
		GetCapsuleComponent()->SetCapsuleRadius(EnemyData.CapsuleRadius);
	}

	RunSpeed = EnemyData.RunSpeed;
	WalkSpeed = EnemyData.WalkSpeed;
	SwitchRunSpeed_Implementation();

	if(HealthWidgetComponent)
	{
		HealthWidgetComponent->Activate();
		HealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, EnemyData.CapsuleHalfHeight+20.f));
	}

	for(const FEnemyMeleeTraceCollisionProperties& TraceCollisionProperties : EnemyData.MeleeTraceCollisionPropertieses)
	{
		UCapsuleComponent* TraceCollision = NewObject<UCapsuleComponent>(GetMesh());
		if(TraceCollision)
		{
			TraceCollision->RegisterComponent();
			TraceCollisions.Add(TraceCollisionProperties.MeleeTraceType, TraceCollision);
			TraceCollision->SetCollisionProfileName(TEXT("Weapon"));
			TraceCollision->SetCapsuleHalfHeight(TraceCollisionProperties.CapsuleHalfHeight);
			TraceCollision->SetCapsuleRadius(TraceCollisionProperties.CapsuleRadius);
			TraceCollision->SetGenerateOverlapEvents(true);
			TraceCollision->bHiddenInGame = true;
			
			if(GetMesh())
			{
				FString TypeString = FString("");
				const UEnum* EnumPtr = StaticEnum<EMeleeTraceType>();
				if (EnumPtr)
				{
					TypeString = EnumPtr->GetNameStringByValue(static_cast<int64>(TraceCollisionProperties.MeleeTraceType));
				}
				
				FName SocketName = FName(FString::Printf(TEXT("%sWeaponSocket"), *TypeString));
				TraceCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			}
			TraceCollision->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
			
			if(!TraceCollision->OnComponentBeginOverlap.IsBound())
			{
				TraceCollision->OnComponentBeginOverlap.AddDynamic(this, &ASoulLikeEnemy::OnOverlapMeleeTraceCollision);
			}
		}
	}

	BehaviorTree = EnemyData.BehaviorTree;
	USoulLikeAbilitySystemLibrary::SetAIStatusWithTag(this, FSoulLikeGameplayTags::Get().Status_AI_Idle);
}

void ASoulLikeEnemy::OnRep_EnemyData(FEnemyData OldEnemyData)
{
	SetupActorWithEnmeyData();
}

void ASoulLikeEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulLikeEnemy, EnemyData);
	DOREPLIFETIME(ASoulLikeEnemy, SpawnerActor);
}
