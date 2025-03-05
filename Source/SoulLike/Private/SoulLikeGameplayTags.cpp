
#include "SoulLikeGameplayTags.h"
#include "GameplayTagsManager.h"

FSoulLikeGameplayTags FSoulLikeGameplayTags::GameplayTags;

void FSoulLikeGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increase Health Point")
	);
	
	GameplayTags.Attributes_Primary_Mind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Mind"),
		FString("Increase Mana Point")
	);

	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"),
		FString("Increase Stamina and Equip Load")
	);

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increase Strength-based Weapon Damage")
	);

	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increase Dexterity-based Weapon Damage")
	);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increase Magic Damage")
	);
	
	GameplayTags.Attributes_Secondary_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Health"),
		FString("Health Point")
	);
	
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum Health Point")
	);

	GameplayTags.Attributes_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Stamina"),
		FString("Stamina Point")
	);

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum Stamina Point")
	);

	GameplayTags.Attributes_Boost_HealthBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.HealthBoost"),
		FString("MaxHealth Boost")
	);

	GameplayTags.Attributes_Boost_StaminaBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.StaminaBoost"),
		FString("MaxStamina Boost")
	);

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag Mouse Left")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag Mouse Right")
	);

	GameplayTags.bInit = true;
}
