// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Interface/InteractionInterface.h"
#include "SoulLikeAbilityTypes.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "SoulLikeCharacterBase.generated.h"

class UEquipmentItemInstance;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class UInventoryComponent;
class UMotionWarpingComponent;
class UCharacterTrajectoryComponent;
class USoulLikeAnimInstanceBase;
class UWidgetComponent;
struct FDamageEffectParams;

UCLASS()
class SOULLIKE_API ASoulLikeCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IInteractionInterface, public IGenericTeamAgentInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	ASoulLikeCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/* Combat Interface */
	virtual void EquipOnCharacter_Implementation(AActor* Equipment, FName SocketName) override;
	virtual UEquipmentItemInstance* GetCurrentWeapon_Implementation() override;
	virtual UItemData* GetCurrentWeaponItemData_Implementation() override;
	virtual void SetupCurrentWeaponItemType_Implementation(const FGameplayTag& ItemType) override;
	virtual UAnimMontage* GetCurrentWeaponAttackMontage_Implementation() override;
	virtual UAnimMontage* GetCurrentWeaponRiposteMontage_Implementation() override;
	virtual UEquipmentItemInstance* GetCurrentTool_Implementation() override;
	virtual UAnimMontage* GetCurrentToolMontage_Implementation() override;
	virtual UAnimMontage* EvaluateRollingMontage_Implementation() override;
	virtual void UsingTool_Implementation(URegisterableItemInstance* ItemInstance) override;

	virtual void NextSlot_Implementation(const FGameplayTag& SlotTag) override;
	virtual void MeleeTrace_Implementation(const FTransform& TipStartRelativeToParent, const FTransform& TipEndRelativeToParent, float Radius, bool bRelativeLoc) override;
	virtual void ApplyDamageToTarget_Implementation(AActor* Target, float DamageBoost) override;
	virtual void ClearIgnoreActors_Implementation() override;
	virtual bool TryBackstab_Implementation() override;
	virtual void SetStrafeMode_Implementation(bool bStrafe) override;
	virtual void SwitchWalkSpeed_Implementation() const override;
	virtual void SwitchRunSpeed_Implementation() const override;
	virtual void SetWeaponCollisionEnable_Implementation(bool bEnable, EMeleeTraceType MeleeTraceType, FName InAbilityIndex);
	virtual FOnDeathSignature& GetOnDeathDelegate() override;

	virtual bool GetLadderMirror_Implementation() const override;
	virtual void SetLadderMirror_Implementation(bool InMirror) override;
	virtual bool GetMirror_Implementation() const override;
	virtual void SetMirror_Implementation(bool InMirror) override;

	virtual void SetMovementMode_Implementation(EMovementMode MovementMode) override;
	virtual float GetLadderMoveInput_Implementation() const override;

	virtual void ToggleTargetMark_Implementation() override;

	//GenericTeamAgent Interface
	virtual FGenericTeamId GetGenericTeamId() const override{ return TeamId; }
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { this->TeamId = TeamID; }

	//GameplayTagAsset Interface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	
	//Motion Warping Func
	virtual void SetWarpingLocationAndRotation(FVector Location, FRotator Rotation);
	virtual void SetWarpingLocation();
	virtual void SetWarpingRotation(FRotator TargetRotation);
	
	void SetControllerRotation(const FRotator& Rotation) const;

	UFUNCTION(Server, Reliable)
	void Server_SetStrafeMode(bool bStrafe);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetStrafeMode(bool bStrafe);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SendHitReactTriggerEvent(const FGameplayEventData Payload);
	
	virtual void DeathToCharacter(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	virtual void DisableCharacter();

	UFUNCTION()
	void OnDeathMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	FGameplayTagContainer GetCurrentWeaponItemTypes() const;

	//HealthWidget Delegate
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnChangedVisibleSignature OnChangedVisible;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

protected:

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/**
	 * Ability의 기본 설정을 하는 함수
	 */
	virtual void InitAbilityActorInfo();

	/**
	 * Attribute를 Init하는 함수
	 */
	virtual void InitializeDefaultAttributes() const;

	/**
	 * StartupAbilities 를 부여하는 함수
	 * TODO
	 * 나중에 Enemy와 Character를 분리해야함
	 */
	void GiveAbilities();
	
	/**
	 * 자기자신에게 Effect를 Apply하는 함수
	 * @param GameplayEffectClass Effct의 Class
	 * @param Level Effect의 Level
	 */
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	
	virtual void SetupDamageParams(FDamageEffectParams& DamageEffectParams, float DamageBoost = 1.f);

	UFUNCTION(Client, Reliable)
	void ClientMeleeTrace(const FTransform& TraceStartRelativeTransform, const FTransform& TraceEndRelativeTransform, float Radius, bool bRelativeLoc);

	UFUNCTION(Server, Reliable)
	void ServerMeleeTrace(const FTransform& TraceStartRelativeTransform, const FTransform& TraceEndRelativeTransform, float Radius, bool bRelativeLoc);
	
	void TryMeleeTrace(const FTransform& TraceStartTransform, const FTransform& TraceEndTransform, float Radius, bool bRelativeLoc = true);
	
	UFUNCTION(Server, Reliable)
	void Server_ApplyDamageToTarget(AActor* Target, float  DamageBoost = 1.f);
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<UWidgetComponent> TargetLockMarkWidgetComponent;

	/**
	 * Movement Properties
	 */
	UPROPERTY()
	float RunSpeed = 600.f;
	
	UPROPERTY()
	float WalkSpeed = 200.f;

	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FullHealthEffectClass;

	// UPROPERTY()
	// bool bReset = false;

	UPROPERTY()
	FName AbilityIndex = FName("");
	
private:

	/**
	 * AnimInstance
	 */
	USoulLikeAnimInstanceBase* GetSL_AnimInstance() const;
		
	/**
	 * MeleeTrace
	 */
	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	/**
	 * Current Weapon ItemTag
	 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	FGameplayTagContainer CurrentWeaponItemTypes;
	
	/**
	 * 캐릭터가 시작할 때 부여받는 기본 Attribute를 제공하는 Effect
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalRegeneration;

	/**
	 * 캐릭터가 시작할 때 부여받는 기본 Ability
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/**
	 * HitImmunity State Tags
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	FGameplayTagContainer HitImmunityStateTags;

	//IGenericTeamAgentInterface
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	FGenericTeamId TeamId;

	//DamageEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
};