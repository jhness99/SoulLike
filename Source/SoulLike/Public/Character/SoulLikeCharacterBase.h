// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "SoulLikeCharacterBase.generated.h"

class UInventoryItemInstance;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class UInventoryComponent;

UCLASS()
class SOULLIKE_API ASoulLikeCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	ASoulLikeCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* Combat Interface */
	virtual void EquipOnCharacter_Implementation(AActor* Equipment) override;
	virtual UInventoryItemInstance* GetCurrentWeapon_Implementation() override;

protected:

	virtual void BeginPlay() override;

	/**
	 * Ability의 기본 설정을 하는 함수
	 */
	virtual void InitAbilityActorInfo();

	/**
	 * Attribute를 Init하는 함수
	 */
	void InitializeDefaultAttributes() const;

	/**
	 * StartupAbilities 를 부여하는 함수
	 * TODO
	 * 나중에 Enemy와 Character를 분리해야함
	 */
	void GiveAbilities() const;
	
	/**
	 * 자기자신에게 Effect를 Apply하는 함수
	 * @param GameplayEffectClass Effct의 Class
	 * @param Level Effect의 Level
	 */
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	
private:
	
	/**
	 * 캐릭터가 시작할 때 부여받는 기본 Attribute를 제공하는 Effect
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/**
	 * 캐릭터가 시작할 때 부여받는 기본 Ability
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
