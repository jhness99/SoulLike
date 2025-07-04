// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SoulLikeAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"

#include "Interface/CombatInterface.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"


ASoulLikeAIController::ASoulLikeAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if(SightConfig)
	{
		SightConfig->SightRadius = 700.f;
		SightConfig->LoseSightRadius = 700.f;
		SightConfig->PeripheralVisionAngleDegrees = 65.f;
		SightConfig->SetMaxAge(2.f);
		
		PerceptionComponent->ConfigureSense(*SightConfig);
	}
	
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	if(DamageConfig)
	{
		DamageConfig->SetMaxAge(5.f);

		PerceptionComponent->ConfigureSense(*DamageConfig);
	}
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ASoulLikeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(const IGenericTeamAgentInterface* PawnTeamAgent = Cast<const IGenericTeamAgentInterface>(InPawn))
	{
		SetGenericTeamId(PawnTeamAgent->GetGenericTeamId());
	}
	
	GetGenericTeamId().SetAttitudeSolver(&TeamAttitudeSolver);
}

void ASoulLikeAIController::OnUnPossess()
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
	{
		for(const FGameplayTag& Tag : DisableTargetFocusTags)
		{
			ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}
	
	Super::OnUnPossess();
}

void ASoulLikeAIController::BeginPlay()
{
	Super::BeginPlay();

	if(GetAIPerceptionComponent())
	{
		GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASoulLikeAIController::OnTargetPerceptionUpdated);
		GetAIPerceptionComponent()->OnTargetPerceptionForgotten.AddDynamic(this, &ASoulLikeAIController::OnTargetPerceptionForgotten);
	}
}

UBehaviorTreeComponent* ASoulLikeAIController::GetBehaviorComponent() const
{
	return BehaviorTreeComponent;
}

ETeamAttitude::Type ASoulLikeAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	UE_LOG(LogTemp, Warning, TEXT("GetTeamAttitudeTowards's other actor is %s"), *Other.GetName());
	return Super::GetTeamAttitudeTowards(Other);
}

ETeamAttitude::Type ASoulLikeAIController::TeamAttitudeSolver(FGenericTeamId A, FGenericTeamId B)
{
	//100일경우 중립상태
	//만약 피격(Damage)를 일정이상 받는다면 ID영구적으로 변경함(적이됨)
	if(A.GetId() == 100 && B.GetId() == 0)
	{
		return ETeamAttitude::Neutral;
	}
	return A != B ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
}

void ASoulLikeAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this))
	{
		if(ASC->HasMatchingGameplayTag(FSoulLikeGameplayTags::Get().Status_Action)) return;
	}
	if(Actor->Implements<UCombatInterface>())
	{
		if(GetBlackboardComponent()->GetValueAsObject(FName("TargetActor")) != Actor)
		{
			//SetFocus(Actor);
			USoulLikeAbilitySystemLibrary::SetAIStatusWithTag(GetPawn(), FSoulLikeGameplayTags::Get().Status_AI_Chasing);
		}
		GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), Actor);
	}
}

void ASoulLikeAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	ClearFocus(EAIFocusPriority::Gameplay);
	GetBlackboardComponent()->ClearValue(FName("TargetActor"));
	USoulLikeAbilitySystemLibrary::SetAIStatusWithTag(GetPawn(), FSoulLikeGameplayTags::Get().Status_AI_Idle);
}
