#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Info.h"
#include "GameplayTagContainer.h"
#include "SoulLikeGameplayTags.h"
#include "SoulLikeItemTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FWeaponDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 0;
};

USTRUCT(BlueprintType)
struct FWeaponRequirement
{
	GENERATED_BODY()

	FWeaponRequirement(){}

	FWeaponRequirement(FGameplayTag InAttributeTag, float InValue) : AttributeTag(InAttributeTag), Value(InValue) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FStatusEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusEffectType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StatusPoint = 0;
};

USTRUCT(BlueprintType)
struct FAttributeBoostInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeBoostTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ItemType = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UpgradeLevel = 0;
};

USTRUCT(BlueprintType)
struct FSL_ItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description = FString();
};

USTRUCT(BlueprintType)
struct FSL_ToolData : public FSL_ItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsume = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UsingMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> UsingEffect = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_AccessoryData : public FSL_ItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeBoostInfo> AttributeBoostInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BoostEffect = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_EquipmentData : public FSL_ItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight = 0.f;
};

USTRUCT(BlueprintType)
struct FSL_WeaponData : public FSL_EquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim Montage")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim Montage")
	TSoftObjectPtr<UAnimMontage> RiposteMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Stamina = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStatusEffectInfo> StatusEffectInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponRequirement> WeaponRequirements;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponDamageInfo> DamageInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackForce = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Toughness = 0.f;
};

USTRUCT(BlueprintType)
struct FSL_GearData : public FSL_EquipmentData
{
	GENERATED_BODY()
	
};

UCLASS(BlueprintType)
class UItemData : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void Init(const FSL_ItemData* ItemData)
	{
		ItemID = ItemData->ItemID;
		ItemName = ItemData->ItemName;
		Image = ItemData->Image;
		ItemType = ItemData->ItemType;
		Description = ItemData->Description;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description = FString();
};

UCLASS(BlueprintType)
class UToolData : public UItemData
{
	GENERATED_BODY()
	
public:

	virtual void Init(const FSL_ItemData* ItemData) override
	{
		Super::Init(ItemData);

		const FSL_ToolData* ToolData = static_cast<const FSL_ToolData*>(ItemData);

		if(ToolData == nullptr) return;
		
		bConsume = ToolData->bConsume;
		UsingMontage = ToolData->UsingMontage;
		UsingEffect = ToolData->UsingEffect;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsume = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UsingMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> UsingEffect = nullptr;
};

UCLASS(BlueprintType)
class UAccessoryData : public UItemData
{
	GENERATED_BODY()
	
public:

	virtual void Init(const FSL_ItemData* ItemData) override
	{
		Super::Init(ItemData);

		const FSL_AccessoryData* AccessoryData = static_cast<const FSL_AccessoryData*>(ItemData);
		if(AccessoryData == nullptr) return;

		BoostEffect = AccessoryData->BoostEffect;
		AttributeBoostInfos = AccessoryData->AttributeBoostInfos;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BoostEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeBoostInfo> AttributeBoostInfos;
};

UCLASS(BlueprintType)
class UEquipmentData : public UItemData
{
	GENERATED_BODY()
	
public:
	
	virtual void Init(const FSL_ItemData* ItemData) override
	{
		Super::Init(ItemData);

		const FSL_EquipmentData* EquipmentData = static_cast<const FSL_EquipmentData*>(ItemData);

		if(EquipmentData == nullptr) return;
		
		ItemMesh = EquipmentData->ItemMesh;
		Weight = EquipmentData->Weight;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight = 0.f;
};

UCLASS(BlueprintType)
class UWeaponData : public UEquipmentData
{
	GENERATED_BODY()

public:

	virtual void Init(const FSL_ItemData* ItemData) override
	{
		Super::Init(ItemData);
		const FSL_WeaponData* WeaponData = static_cast<const FSL_WeaponData*>(ItemData);

		if(WeaponData == nullptr) return;

		AttackMontage = WeaponData->AttackMontage;
		RiposteMontage = WeaponData->RiposteMontage.LoadSynchronous();
		Radius = WeaponData->Radius;
		Stamina = WeaponData->Stamina;
		DamageInfos = WeaponData->DamageInfos;
		StatusEffectInfos = WeaponData->StatusEffectInfos;
		
		WeaponRequirements.Add(FSoulLikeGameplayTags::Get().Attributes_Primary_Strength, 0);
		WeaponRequirements.Add(FSoulLikeGameplayTags::Get().Attributes_Primary_Dexterity, 0);
		WeaponRequirements.Add(FSoulLikeGameplayTags::Get().Attributes_Primary_Intelligence, 0);

		for(const FWeaponRequirement& Requirement : WeaponData->WeaponRequirements)
		{
			int32& Value = WeaponRequirements.FindChecked(Requirement.AttributeTag);
			Value = Requirement.Value;
		}

		DamageType = WeaponData->DamageType;
		AttackForce = WeaponData->AttackForce;
		Toughness = WeaponData->Toughness;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim Montage")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim Montage")
	TObjectPtr<UAnimMontage> RiposteMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Stamina = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStatusEffectInfo> StatusEffectInfos;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, int32> WeaponRequirements;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FWeaponDamageInfo> DamageInfos;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag DamageType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AttackForce = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Toughness = 0.f;
};

UCLASS(BlueprintType)
class UGearData : public UEquipmentData
{
	GENERATED_BODY()

public:
	
	virtual void Init(const FSL_ItemData* ItemData) override
	{
		Super::Init(ItemData);

		const FSL_GearData* GearData = static_cast<const FSL_GearData*>(ItemData);
	}
};