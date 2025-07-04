// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/RegisterableItemInstance.h"

#include "Net/UnrealNetwork.h"

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
