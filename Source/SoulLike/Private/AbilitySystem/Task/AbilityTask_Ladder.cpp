// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/AbilityTask_Ladder.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Interface/CombatInterface.h"

#include "SoulLikeGameplayTags.h"


UAbilityTask_Interaction* UAbilityTask_Ladder::CreateLadderTask(UGameplayAbility* OwningAbility, AActor* AvatarActor,
                                                                const FInteractionTaskInfo& InteractionTaskInfo)
{
	UAbilityTask_Ladder* LadderTask = NewAbilityTask<UAbilityTask_Ladder>(OwningAbility);

	LadderTask->InteractionTaskInfo = InteractionTaskInfo;
	LadderTask->bTickingTask = true;
	
	return LadderTask;
}

void UAbilityTask_Ladder::Activate()
{
	Super::Activate();
	
	//위 아래의 움직임을 구현하면서 RootMotion을 사용하기 위해 Fly모드로 전환
	if(Ability && Ability->GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetMovementMode(Ability->GetAvatarActorFromActorInfo(), EMovementMode::MOVE_Flying);
	}

	//Blend로 인한 RootMotion 이동거리 차이를 없에기 위해 BlendOut없앰
	InteractionTaskInfo.Montage->BlendOut.SetBlendTime(0.0f);
	bCanReceiveInput = false;
}

void UAbilityTask_Ladder::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//Input을 받을 수 있는 상태이면서 ClimbLoop Animation이 활성화 되지 않았을경우
	//ClimbMontage의 중복작동 막음
	if(bCanReceiveInput &&
		(!IsValid(LadderClimbMontageTask) || !LadderClimbMontageTask->IsActive()))
	{
		FName SectionName = FName("");
		FString Direction = FString("");
		FString RL = FString("");
		
		if(Ability && Ability->GetAvatarActorFromActorInfo()->Implements<UCombatInterface>() && Ability->GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
		{
			//InputValue를 가져옴
			float InputValue = ICombatInterface::Execute_GetLadderMoveInput(Ability->GetAvatarActorFromActorInfo());
			//입력이 없다면 함수종료
			if(InputValue == 0.f) return;
			//종결 Montage를 작동해야 하는지에 대한 여부를 확인하기 위해 현재 Overlay된 InteractionActor의 Tag가져옴
			FGameplayTag InteractionTag = IInteractionInterface::Execute_GetInteractionTag(Ability->GetAvatarActorFromActorInfo());
			
			if(InputValue > 0.f)
			{
				Direction = FString("Up");
				//만약 Overlap된 Interaction의 방향이 아래일경우, 위로 올라가는 종결Montage 재생
				if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Down))
				{
					bCanReceiveInput = false;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Ability->GetAvatarActorFromActorInfo(), InteractionTag, FGameplayEventData());
					ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), false);
					ICombatInterface::Execute_SetMirror(Ability->GetAvatarActorFromActorInfo(), bIsRight);
					return;
				}
			}
			else if(InputValue < 0.f)
			{
				Direction = FString("Down");
				//만약 Overlap된 Interaction의 방향이 위일경우, 아래로 내려가는 종결Montage 재생
				if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Up))
				{
					bCanReceiveInput = false;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Ability->GetAvatarActorFromActorInfo(), InteractionTag, FGameplayEventData());
					ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), false);
					ICombatInterface::Execute_SetMirror(Ability->GetAvatarActorFromActorInfo(), bIsRight);
					return;
				}
			}
			//현재 상태에 따른 LadderIdle Animation Mirror설정
			//ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), !bIsRight);
		}
		
		if(bIsRight)
		{
			RL = FString("R");
		}
		else
		{
			RL = FString("L");
		}
		//현재 LadderIdle Mirror상태 스위치
		bIsRight = !bIsRight;
		
		SectionName = FName(*FString::Printf(TEXT("%sLoop_%s_%s"), *InteractionTaskInfo.SectionName, *Direction, *RL));
		
		LadderClimbMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, FName("LadderLoop"), InteractionTaskInfo.Montage, 1, SectionName);
		LadderClimbMontageTask->ReadyForActivation();
	}
}

FName UAbilityTask_Ladder::GetStartSectionName()
{
	FName Direction = GetOverlapLocationName();
	
	return FName(*FString::Printf(TEXT("%s_%s"), *Super::GetStartSectionName().ToString(), *Direction.ToString()));
}

FName UAbilityTask_Ladder::GetEndSectionName()
{
	FName Direction = GetOverlapLocationName();
	
	return FName(*FString::Printf(TEXT("%s_%s"), *Super::GetEndSectionName().ToString(), *Direction.ToString()));
}

void UAbilityTask_Ladder::OnCompletedStartMontage()
{
	Super::OnCompletedStartMontage();

	//시작 Montage가 끝날 때, 입력을 받기 시작
	bCanReceiveInput = true;
}

void UAbilityTask_Ladder::OnCompletedEndMontage()
{
	if(Ability && Ability->GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetMovementMode(Ability->GetAvatarActorFromActorInfo(), EMovementMode::MOVE_Walking);
		ICombatInterface::Execute_SetLadderMirror(Ability->GetAvatarActorFromActorInfo(), false);
		ICombatInterface::Execute_SetMirror(Ability->GetAvatarActorFromActorInfo(), false);
	}
	//종결 Montage의 자연스러운 종료를 위해 BlendOut설정
	InteractionTaskInfo.Montage->BlendOut.SetBlendTime(0.2f);
	Super::OnCompletedEndMontage();
}

FName UAbilityTask_Ladder::GetOverlapLocationName() const
{
	if(Ability && Ability->GetAvatarActorFromActorInfo()->Implements<UInteractionInterface>())
	{
		FGameplayTag InteractionTag = IInteractionInterface::Execute_GetInteractionTag(Ability->GetAvatarActorFromActorInfo());
		if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Up))
		{
			return FName("Up");
		}
		if(InteractionTag.MatchesTagExact(FSoulLikeGameplayTags::Get().Interaction_Ladder_Down))
		{
			return FName("Down");
		}
	}
	return FName("");
}

