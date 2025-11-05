// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/RegisterableItemInstance.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

void URegisterableItemInstance::UsingItem()
{
	// if(GetToolData()->UsingEffect->IsValidLowLevel())
	// {
	// 	if(USoulLikeAbilitySystemComponent* SL_ASC = Cast<USoulLikeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	// 	{
	// 		SL_ASC->ApplyEffectToSelf(GetToolData()->UsingEffect, 1);
	// 	}
	// 	//ApplyEffectToSelf(ItemInstance->GetToolData()->UsingEffect, 1);
	// }
}

UAnimMontage* URegisterableItemInstance::GetUsingMontage() const
{
	if(UToolData* RegisterableItemData = Cast<UToolData>(ItemData))
	{
		return RegisterableItemData->UsingMontage;
	}
	
	return nullptr;
}

void URegisterableItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URegisterableItemInstance, RegistInfo);
}
