// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulLikeEnemy.h"

#include "AbilitySystem/SoulLikeAbilitySystemComponent.h"
#include "AbilitySystem/SoulLikeAttributeSet.h"

#include "Inventory/InventoryComponent.h"

#include "Components/WidgetComponent.h"
#include "UI/Widgets/SoulLikeUserWidget.h"

ASoulLikeEnemy::ASoulLikeEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulLikeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USoulLikeAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
}

void ASoulLikeEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if(USoulLikeUserWidget* UserWidget = Cast<USoulLikeUserWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}
}

void ASoulLikeEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
