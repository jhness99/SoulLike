// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeFunctionLibrary.h"

#include "GameFramework/PlayerController.h"
#include "Player/SoulLikePlayerState.h"

#include "Camera/CameraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Abilities/TargetLockGameplayAbility.h"

#include "Game/SoulLikeGameInstance.h"

#include "Character/SoulLikeCharacterBase.h"

#include "UI/HUD/SoulLikeHUD.h"

#include "Inventory/Data/ItemDataAsset.h"
#include "Inventory/Data/DamageTypeInfo.h"

#include "Character/SoulLikeCharacter.h"

#include "Kismet/GameplayStatics.h"


class UCameraComponent;

bool USoulLikeFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                          ASoulLikeHUD*& OutSoulLikeHUD, FWidgetControllerParams& OutWCParams)
{
	UE_LOG(LogTemp, Warning, TEXT("MakeWidgetControllerParams GetPlayerController 시도"));
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("MakeWidgetControllerParams GetHUD 시도"));
		OutSoulLikeHUD = Cast<ASoulLikeHUD>(PC->GetHUD());
		if(OutSoulLikeHUD != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MakeWidgetControllerParams GetPlayerState 시도"));
			ASoulLikePlayerState* PS = PC->GetPlayerState<ASoulLikePlayerState>();
			if(PS == nullptr) return false;
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			UE_LOG(LogTemp, Warning, TEXT("MakeWidgetControllerParams 성공"));
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* USoulLikeFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetOverlayWidgetController(Params);
	}
	return nullptr;
}

UInventoryWidgetController* USoulLikeFunctionLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	UE_LOG(LogTemp, Warning, TEXT("GetInventoryWidgetController 시도"));
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetInventoryWidgetController(Params);
	}
	UE_LOG(LogTemp, Warning, TEXT("GetInventoryWidgetController 실패"));
	return nullptr;
}

UAttributeSetWidgetController* USoulLikeFunctionLibrary::GetAttributeSetWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetAttributeSetWidgetController(Params);
	}
	return nullptr;
}

ULevelUpMenuWidgetController* USoulLikeFunctionLibrary::GetLevelUpMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetLevelUpMenuWidgetController(Params);
	}
	return nullptr;
}

UConfirmMenuWidgetController* USoulLikeFunctionLibrary::GetConfirmMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetConfirmMenuWidgetController(Params);
	}
	return nullptr;
}

UUpgradeMenuWidgetController* USoulLikeFunctionLibrary::GetUpgradeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetUpgradeMenuWidgetController(Params);
	}
	return nullptr;
}

UKeybindMenuWidgetController* USoulLikeFunctionLibrary::GetKeybindMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ASoulLikeHUD* SoulLikeHUD;
	const bool bSuccessfulMakingParams = MakeWidgetControllerParams(WorldContextObject, SoulLikeHUD, Params);
	if(bSuccessfulMakingParams)
	{
		return SoulLikeHUD->GetKeybindMenuWidgetController(Params);
	}
	return nullptr;
}

USoulLikeGameInstance* USoulLikeFunctionLibrary::GetSL_GameInstance(const UObject* WorldContextObject)
{
	if(USoulLikeGameInstance* GameInstance = Cast<USoulLikeGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return GameInstance;
	}
	return nullptr;
}

UUISubSystem* USoulLikeFunctionLibrary::GetUISubSystem(const UObject* WorldContextObject)
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GI->GetSubsystem<UUISubSystem>();
	}
	return nullptr;
}

UAbilityInfo* USoulLikeFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		return GameInstance->AbilityInfoDataAsset;
	}
	return nullptr;
}

UDamageTypeInfo* USoulLikeFunctionLibrary::GetDamageTypeInfo(const UObject* WorldContextObject)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		return GameInstance->DamageTypeInfoDataAsset;
	}
	return nullptr;
}

UAttributeInfo* USoulLikeFunctionLibrary::GetAttributeInfo(const UObject* WorldContextObject)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		return GameInstance->AttributeInfoDataAsset;
	}
	return nullptr;
}

FSoulLikeAbilityInfo USoulLikeFunctionLibrary::GetAbilityInfoForTag(const UObject* WorldContextObject,
                                                                    const FGameplayTag& Tag)
{
	if(UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject))
	{
		return AbilityInfo->FindAbilityInfoForTag(Tag);
	}
	return FSoulLikeAbilityInfo();
}

FSoulLikeDamageTypeInfo USoulLikeFunctionLibrary::GetDamageTypeInfoForTag(const UObject* WorldContextObject,
	const FGameplayTag& Tag)
{
	if(UDamageTypeInfo* DamageTypeInfo = GetDamageTypeInfo(WorldContextObject))
	{
		return DamageTypeInfo->FindDamageTypeInfoWithTag(Tag);
	}
	return FSoulLikeDamageTypeInfo();
}

FSoulLikeAttributeInfo USoulLikeFunctionLibrary::GetAttributeInfoForTag(const UObject* WorldContextObject,
	const FGameplayTag& Tag)
{
	if(UAttributeInfo* AttributeInfo = GetAttributeInfo(WorldContextObject))
	{
		return AttributeInfo->FindAttributeInfoForTag(Tag);
	}
	return FSoulLikeAttributeInfo();
}

const FInteractionTaskInfo USoulLikeFunctionLibrary::GetInteractionTaskInfoForTag(const UObject* WorldContextObject,
                                                                                  const FGameplayTag& Tag)
{
	if(UAbilityInfo* AbilityInfo = GetAbilityInfo(WorldContextObject))
	{
		return AbilityInfo->FindInteractionTaskInfoForTag(Tag);
	}
	return FInteractionTaskInfo();
}

float USoulLikeFunctionLibrary::GetBoostedDamageFromAttribute(const UObject* WorldContextObject, UAttributeSet* AS, const FGameplayTag& DamageTag, float Damage)
{
	if(AS == nullptr) return Damage;
	if(UAttributeInfo* AttributeInfo = GetAttributeInfo(WorldContextObject))
	{
		float AttributeValue = USoulLikeAttributeSet::TagsToAttributes[AttributeInfo->DamageTypeTagToBoostAttributes[DamageTag]]().GetNumericValue(AS);
		Damage *= ((AttributeValue * 0.02) + 1);
	}
	return Damage;
}

FString USoulLikeFunctionLibrary::GetTagNameFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		if(GameInstance->ItemDataAsset)
		{
			return GameInstance->ItemDataAsset->FindTagNameFromGameplayTag(Tag);
		}
	}
	return FString();
}

FString USoulLikeFunctionLibrary::GetItemTypeFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		if(GameInstance->ItemDataAsset)
		{
			return GameInstance->ItemDataAsset->FindItemTypeFromGameplayTag(Tag);
		}
	}
	return FString();
}

FString USoulLikeFunctionLibrary::GetWeaponTypeFromGameplayTag(const UObject* WorldContextObject,
                                                               const FGameplayTag& Tag)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		if(GameInstance->ItemDataAsset)
		{
			return GameInstance->ItemDataAsset->FindWeaponTypeFromGameplayTag(Tag);
		}
	}
	return FString();
}

FString USoulLikeFunctionLibrary::GetToolTypeFromGameplayTag(const UObject* WorldContextObject, const FGameplayTag& Tag)
{
	if(USoulLikeGameInstance* GameInstance = GetSL_GameInstance(WorldContextObject))
	{
		if(GameInstance->ItemDataAsset)
		{
			return GameInstance->ItemDataAsset->FindToolTypeFromGameplayTag(Tag);
		}
	}
	return FString();
}

TArray<FInventoryListItem>& USoulLikeFunctionLibrary::GetInvenoryListItemsRef(FInventoryList InventoryList)
{
	return InventoryList.GetItemsRef();
}

const TArray<FInventoryListItem>& USoulLikeFunctionLibrary::GetInvenoryListItems(const FInventoryList& InventoryList)
{
	return InventoryList.GetItems();
}

AActor* USoulLikeFunctionLibrary::ClosestActorWithHitResults(const UObject* WorldContextObject,
                                                             const TArray<FHitResult>& HitResults)
{
	const AActor* Instigator = Cast<AActor>(WorldContextObject);
	if(Instigator == nullptr) return nullptr;
	
	AActor* ClosestActor = nullptr;
	FVector DistanceToClosestActor = FVector::ZeroVector;
	for(const FHitResult& HitResult : HitResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("%hs , HitActor is %s"), __FUNCTION__, *HitResult.GetActor()->GetName())
		if(ClosestActor == nullptr)
		{
			ClosestActor = HitResult.GetActor();
			DistanceToClosestActor = ClosestActor->GetActorLocation() - Instigator->GetActorLocation();
		}
		else
		{
			FVector DistanceToActor = HitResult.GetActor()->GetActorLocation() - Instigator->GetActorLocation();
			if(DistanceToActor.Length() < DistanceToClosestActor.Length())
			{
				ClosestActor = HitResult.GetActor();
				DistanceToClosestActor = DistanceToActor;
			}
		}
	}
	return ClosestActor;
}

bool USoulLikeFunctionLibrary::CheckTargetOnViewport(const AActor* TargetActor, float Tolerance){
	if(TargetActor == nullptr) return false;
	
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (PrimitiveComponent == nullptr)
		return false;

	UWorld* World = PrimitiveComponent->GetWorld();
	
	return (World) ? (World->GetTimeSeconds() - PrimitiveComponent->GetLastRenderTimeOnScreen() <= Tolerance) : false;
}

bool USoulLikeFunctionLibrary::CheckDistanceToTarget(const AActor* Instigator, const AActor* TargetActor,
	float Distance)
{
	if(Instigator == nullptr || TargetActor == nullptr) return false;
	
	float DistanceToTarget = FVector::Distance(Instigator->GetActorLocation(), TargetActor->GetActorLocation());

	return DistanceToTarget < Distance;
}

bool USoulLikeFunctionLibrary::SetupTraceProperties(const AActor* Instigator, FVector& TraceStartLocation, FVector& TraceEndLocation,
                                                    FVector& TraceForwardVector, float TraceLength)
{
	UCameraComponent* CameraComponent = nullptr;
	
	if(const ASoulLikeCharacter* Character = Cast<ASoulLikeCharacter>(Instigator))
	{
		CameraComponent = Character->GetPlayerCameraComponent();
	}

	if(CameraComponent == nullptr) return false;
	
	TraceStartLocation = CameraComponent->GetComponentLocation();
	TraceForwardVector = CameraComponent->GetForwardVector();
	TraceEndLocation = TraceStartLocation + TraceForwardVector * TraceLength;

	return true;
}

bool USoulLikeFunctionLibrary::SingleTraceFromCameraLocation(const AActor* Instigator, FHitResult& HitResult,
                                                             FTraceProperties TraceProperties, bool bDebug)
{
	FVector TraceStartLocation = FVector::ZeroVector;
	FVector TraceEndLocation = FVector::ZeroVector;
	FVector TraceForwardVector = FVector::ZeroVector;
	
	if(!SetupTraceProperties(Instigator, TraceStartLocation, TraceEndLocation, TraceForwardVector, TraceProperties.TraceLength)) return false;
	
	FCollisionQueryParams Params(NAME_None, false, Instigator);

	UWorld* World = Instigator->GetWorld();
	if(World == nullptr) return false;
	
	bool bResult = World->SweepSingleByChannel(
		HitResult,
		TraceStartLocation + (TraceForwardVector * TraceProperties.TraceSphereRadius),
		TraceEndLocation - (TraceForwardVector * TraceProperties.TraceSphereRadius),
		FQuat::Identity,
		TraceProperties.TraceCollisionChannel,
		FCollisionShape::MakeSphere(TraceProperties.TraceSphereRadius),
		Params);

	if(bResult)
	{
		if(HitResult.GetActor())
		{
			bResult = HitResult.GetActor()->IsA(ASoulLikeCharacterBase::StaticClass());
			
			if(bDebug)
			{
				FVector TraceVec = TraceForwardVector;
				FVector Center = TraceStartLocation + TraceForwardVector * TraceProperties.TraceLength / 2;
				float HalfHeight = TraceProperties.TraceLength / 2.f;
				FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
				FColor DrawColor = bResult ? FColor::Green : FColor::Red;

				DrawDebugCapsule(World,
					Center,
					HalfHeight,
					5.0f,
					CapsuleRot,
					DrawColor,
					false,
					TraceProperties.DebugLifeTime);
			}
			
			return bResult;
		}
	}
	
	return false;
}

bool USoulLikeFunctionLibrary::MultiTraceFromCameraLocation(const AActor* Instigator, TArray<FHitResult>& HitResults,
	FTraceProperties TraceProperties, bool bDebug)
{
	FVector TraceStartLocation = FVector::ZeroVector;
	FVector TraceEndLocation = FVector::ZeroVector;
	FVector TraceForwardVector = FVector::ZeroVector;
	
	if(!SetupTraceProperties(Instigator, TraceStartLocation, TraceEndLocation, TraceForwardVector, TraceProperties.TraceLength)) return false;
	
	FCollisionQueryParams Params(NAME_None, false, Instigator);
	
	// ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel3);
	// TArray<AActor*> IgnoreActors;
	// IgnoreActors.Add(GetAvatarActorFromActorInfo());
	// UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStartLocation + (TraceForwardVector * MultiTraceSphereRadius), TraceEndLocation - (TraceForwardVector * MultiTraceSphereRadius), MultiTraceSphereRadius, TraceTypeQuery, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);

	UWorld* World = Instigator->GetWorld();
	if(World == nullptr) return false;
	
	World->SweepMultiByChannel(
		HitResults,
		TraceStartLocation + (TraceForwardVector * TraceProperties.TraceSphereRadius),
		TraceEndLocation - (TraceForwardVector * TraceProperties.TraceSphereRadius),
		FQuat::Identity,
		TraceProperties.TraceCollisionChannel,
		FCollisionShape::MakeSphere(TraceProperties.TraceSphereRadius),
		Params);
	
	if(bDebug){
		FVector TraceVec = TraceForwardVector;
		FVector Center = TraceStartLocation + TraceForwardVector * TraceProperties.TraceLength / 2;
		float HalfHeight = TraceProperties.TraceLength / 2;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = HitResults.Num() ? FColor::Green : FColor::Red;
	
		DrawDebugCapsule(World,
			Center,
			HalfHeight,
			TraceProperties.TraceSphereRadius,
			CapsuleRot,
			DrawColor,
			false,
			TraceProperties.DebugLifeTime);
	}

	if(HitResults.Num() > 0) return true;
	return false;
}

// bool USoulLikeFunctionLibrary::SetAIStatusWithTag(const AActor* WorldContextActor, const FGameplayTag& StatusTag)
// {
// 	if(UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(WorldContextActor)))
// 	{
// 		const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
// 		FGameplayTagContainer CurrentTags;
// 		ASC->GetOwnedGameplayTags(CurrentTags);
//
// 		for(const FGameplayTag& CurrentTag : CurrentTags)
// 		{
// 			if(CurrentTag.MatchesTag(GameplayTags.Status_AI))
// 			{
// 				ASC->RemoveLooseGameplayTag(CurrentTag);
// 				ASC->RemoveReplicatedLooseGameplayTag(CurrentTag);
// 			}
// 		}
// 		ASC->AddLooseGameplayTag(StatusTag);
// 		ASC->AddReplicatedLooseGameplayTag(StatusTag);
// 		return true;
// 	}
// 	return false;
// }
//
// void USoulLikeFunctionLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
// 	const FGameplayTag& InDamageType)
// {
// 	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
//
// 		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
// 		SoulLikeEffectContext->SetDamageType(DamageType);    
// 	}
// }
//
// void USoulLikeFunctionLibrary::SetAttackForce(FGameplayEffectContextHandle& EffectContextHandle, float InAttackForce)
// {
// 	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
// 		
// 		SoulLikeEffectContext->SetAttackForce(InAttackForce);    
// 	}
// }
//
// void USoulLikeFunctionLibrary::SetToughness(FGameplayEffectContextHandle& EffectContextHandle, float InToughness)
// {
// 	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
// 		
// 		SoulLikeEffectContext->SetToughness(InToughness);    
// 	}
// }
//
// void USoulLikeFunctionLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
// 	FVector InKnockbackForce)
// {
// 	if(FSoulLikeGameplayEffectContext* SoulLikeEffectContext = static_cast<FSoulLikeGameplayEffectContext*>(EffectContextHandle.Get())){
// 		
// 		SoulLikeEffectContext->SetKnockbackForce(InKnockbackForce);    
// 	}
// }
//
// FGameplayEffectContextHandle USoulLikeFunctionLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
// {
// 	const FSoulLikeGameplayTags& GameplayTags = FSoulLikeGameplayTags::Get();
// 	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
//
// 	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
// 	EffectContextHandle.AddSourceObject(SourceAvatarActor);
//
// 	SetDamageType(EffectContextHandle, DamageEffectParams.AttackType);
// 	SetAttackForce(EffectContextHandle, DamageEffectParams.AttackForce);
// 	SetToughness(EffectContextHandle, DamageEffectParams.Toughness);
// 	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
//
// 	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, 1.f, EffectContextHandle);
//
// 	for(const FWeaponDamageInfo& Info : DamageEffectParams.DamageInfos)
// 	{
// 		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Info.DamageType, Info.Damage);
// 	}
//
// 	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
// 	return EffectContextHandle;
// }
