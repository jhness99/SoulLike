#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Info.h"
#include "GameplayTagContainer.h"
#include "SoulLikeItemTypes.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	EWS_Right	UMETA(DisplayName = "오른쪽"),
	EWS_Left	UMETA(DisplayName = "왼쪽")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_None			UMETA(DisplayName = "None"),
	EIT_Item			UMETA(DisplayName = "아이템"),
	EIT_Registerable	UMETA(DisplayName = "장착가능"),
	EIT_Gear			UMETA(DisplayName = "방어구"),
	EIT_Weapon			UMETA(DisplayName = "무기")
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_None UMETA(DisplayName = "None"),
	EET_Gear UMETA(DisplayName = "방어구"),
	EET_Weapon UMETA(DisplayName = "무기")
};

UENUM(BlueprintType)
enum class EGearType : uint8
{
	EGT_None UMETA(DisplayName = "None"),
	EGT_Helmet UMETA(DisplayName = "투구"),
	EGT_Breastplate UMETA(DisplayName = "흉갑"),
	EGT_Leggings UMETA(DisplayName = "각반"),
	EGT_Boots UMETA(DisplayName = "부츠")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None UMETA(DisplayName = "None"),
	EWT_Katana UMETA(DisplayName = "도"),
	EWT_Sword UMETA(DisplayName = "직검")
};

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
struct FStatusEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusEffectType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StatusPoint = 0;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = FName();
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
	EItemType ItemType = EItemType::EIT_None;
};

USTRUCT(BlueprintType)
struct FSL_RegisterableItemData : public FSL_ItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsume = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UsingMontage = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_EquipmentData : public FSL_ItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentType EquipmentType = EEquipmentType::EET_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight = 0.f;
};

USTRUCT(BlueprintType)
struct FSL_WeaponData : public FSL_EquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::EWT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Stamina = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponDamageInfo> DamageInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStatusEffectInfo> StatusEffectInfos;
};

USTRUCT(BlueprintType)
struct FSL_GearData : public FSL_EquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGearType GearType = EGearType::EGT_None;
};

UCLASS(BlueprintType)
class UItemData : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override { return true; }
	
	void Init(const FSL_ItemData& ItemData)
	{
		ItemID = ItemData.ItemID;
		ItemName = ItemData.ItemName;
		Image = ItemData.Image;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};

UCLASS(BlueprintType)
class URegisterableItemData : public UItemData
{
	GENERATED_BODY()
	
public:

	void Init(const FSL_RegisterableItemData& RegisterableItemData)
	{
		ItemID = RegisterableItemData.ItemID;
		ItemName = RegisterableItemData.ItemName;
		Image = RegisterableItemData.Image;
		bConsume = RegisterableItemData.bConsume;
		UsingMontage = RegisterableItemData.UsingMontage;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsume = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UsingMontage = nullptr;
};

UCLASS(BlueprintType)
class UEquipmentData : public UItemData
{
	GENERATED_BODY()
	
public:

	void Init(const FSL_EquipmentData& EquipmentData)
	{
		ItemID = EquipmentData.ItemID;
		ItemName = EquipmentData.ItemName;
		Image = EquipmentData.Image;
		EquipmentType = EquipmentData.EquipmentType;
		ItemMesh = EquipmentData.ItemMesh;
		Weight = EquipmentData.Weight;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentType EquipmentType = EEquipmentType::EET_None;

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

	void Init(const FSL_WeaponData& WeaponData)
	{
		ItemID = WeaponData.ItemID;
		ItemName = WeaponData.ItemName;
		Image = WeaponData.Image;
		EquipmentType = WeaponData.EquipmentType;
		ItemMesh = WeaponData.ItemMesh;
		WeaponType = WeaponData.WeaponType;
		AttackMontage = WeaponData.AttackMontage;
		Radius = WeaponData.Radius;
		Stamina = WeaponData.Stamina;
		DamageInfos = WeaponData.DamageInfos;
		StatusEffectInfos = WeaponData.StatusEffectInfos;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::EWT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Stamina = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponDamageInfo> DamageInfos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStatusEffectInfo> StatusEffectInfos;
};

UCLASS(BlueprintType)
class UGearData : public UEquipmentData
{
	GENERATED_BODY()

public:
	
	void Init(const FSL_GearData& GearData)
	{
		ItemID = GearData.ItemID;
		ItemName = GearData.ItemName;
		Image = GearData.Image;
		EquipmentType = GearData.EquipmentType;
		ItemMesh = GearData.ItemMesh;
		GearType = GearData.GearType;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGearType GearType = EGearType::EGT_None;
};