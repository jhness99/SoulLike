#pragma once

#include "CoreMinimal.h"
#include "SoulLikeItemTypes.generated.h"

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
struct FSL_ItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_RegisterableItemData : public FSL_ItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsume = false;
};

USTRUCT(BlueprintType)
struct FSL_EquipmentData : public FSL_ItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentType EquipmentType = EEquipmentType::EET_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> ItemMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_WeaponData : public FSL_EquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::EWT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;
};

USTRUCT(BlueprintType)
struct FSL_GearData : public FSL_EquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGearType GearType = EGearType::EGT_None;
};

UCLASS(BlueprintType, Blueprintable)
class UTestItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;
};