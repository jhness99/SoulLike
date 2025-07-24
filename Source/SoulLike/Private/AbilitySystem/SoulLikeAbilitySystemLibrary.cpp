// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulLikeAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"

#include "Character/Data/EnemyDataAsset.h"
#include "Game/SoulLikeGameInstance.h"
#include "Game/SoulLikeSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SoulLikeFunctionLibrary.h"
#include "Character/Data/CharacterDataAsset.h"


bool USoulLikeAbilitySystemLibrary::SetAIStatusWithTag(const AActor* WorldContextActor, const FGameplayTag& StatusTag)
{
	if(UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(WorldContextActor)))
	{
		const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
		FGameplayTagContainer CurrentTags;
		ASC->GetOwnedGameplayTags(CurrentTags);

		for(const FGameplayTag& CurrentTag : CurrentTags)
		{
			if(CurrentTag.MatchesTag(GameplayTags.Status_AI))
			{
				ASC->RemoveLooseGameplayTag(CurrentTag);
				ASC->RemoveReplicatedLooseGameplayTag(CurrentTag);
			}
		}
		ASC->AddLooseGameplayTag(StatusTag);
		ASC->AddReplicatedLooseGameplayTag(StatusTag);
		return true;
	}
	return false;
}

void USoulLikeAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, USoulLikeSaveGame* SaveData)
{
	USoulLikeGameInstance* SL_GameInstance = USoulLikeFunctionLibrary::GetSL_GameInstance(WorldContextObject);
	if(SL_GameInstance == nullptr) return;
	
	UCharacterDataAsset* CharacterDataAsset = SL_GameInstance->CharacterDataAsset;
    if(CharacterDataAsset == nullptr) return;

    const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();

    const AActor* SourceAvatarActor = ASC->GetAvatarActor();

    FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(SourceAvatarActor);

    const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterDataAsset->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveData->Vigor);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Mind, SaveData->Mind);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Endurance, SaveData->Endurance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveData->Strength);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Dexterity, SaveData->Dexterity);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveData->Intelligence);
	
    ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

    FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
    SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterDataAsset->SecondaryAttirbutes, 1.f, SecondaryAttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
    VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterDataAsset->VitalAttirbutes, 1.f, VitalAttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalRegenerationContextHandle = ASC->MakeEffectContext();
	VitalRegenerationContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalRegenerationSpecHandle = ASC->MakeOutgoingSpec(CharacterDataAsset->VitalRegeneration, 1.f, VitalRegenerationContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalRegenerationSpecHandle.Data.Get());
}

void USoulLikeAbilitySystemLibrary::InitializeDefaultAttributes_Enemy(const UObject* WorldContextObject,
                                                                      UAbilitySystemComponent* ASC, FEnemyData EnemyData)
{
	if(USoulLikeGameInstance* GameInstance = Cast<USoulLikeGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		if(GameInstance->EnemyDataAssset)
		{
			const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();

			const AActor* SourceAvatarActor = ASC->GetAvatarActor();
			
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(SourceAvatarActor);

			const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameInstance->EnemyDataAssset->EnemyAttributesEffectClass, 1.f, EffectContextHandle);

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Secondary_MaxHealth, EnemyData.MaxHealth);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Secondary_MaxStamina, EnemyData.MaxStamina);
			
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
}

void USoulLikeAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const FGameplayTag& InDamageType)
{
	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){

		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		SoulLikeEffectContext->SetDamageType(DamageType);    
	}
}

void USoulLikeAbilitySystemLibrary::SetAttackForce(FGameplayEffectContextHandle& EffectContextHandle, float InAttackForce)
{
	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
		
		SoulLikeEffectContext->SetAttackForce(InAttackForce);    
	}
}

void USoulLikeAbilitySystemLibrary::SetToughness(FGameplayEffectContextHandle& EffectContextHandle, float InToughness)
{
	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
		
		SoulLikeEffectContext->SetToughness(InToughness);    
	}
}

void USoulLikeAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	FVector InKnockbackForce)
{
	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
		
		SoulLikeEffectContext->SetKnockbackForce(InKnockbackForce);    
	}
}

FGameplayEffectContextHandle USoulLikeAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	IGenericTeamAgentInterface* SourceActor = Cast<IGenericTeamAgentInterface>(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor());
	IGenericTeamAgentInterface* TargetActor = Cast<IGenericTeamAgentInterface>(DamageEffectParams.TargetAbilitySystemComponent->GetAvatarActor());

	if(SourceActor == nullptr || TargetActor  == nullptr) return FGameplayEffectContextHandle();

	if(SourceActor->GetGenericTeamId().GetId() == TargetActor->GetGenericTeamId().GetId())
	{
		return FGameplayEffectContextHandle();
	}

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	SetDamageType(EffectContextHandle, DamageEffectParams.AttackType);
	SetAttackForce(EffectContextHandle, DamageEffectParams.AttackForce);
	SetToughness(EffectContextHandle, DamageEffectParams.Toughness);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, 1.f, EffectContextHandle);

	for(const FWeaponDamageInfo& Info : DamageEffectParams.DamageInfos)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Info.DamageType, Info.Damage);
	}

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}
