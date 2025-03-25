
#include "SoulLikeGameplayTags.h"
#include "GameplayTagsManager.h"

FSoulLikeGameplayTags FSoulLikeGameplayTags::GameplayTags;

void FSoulLikeGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * Primary Attribute
	 */
	
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

	/**
	 * Secondary Attribute
	 */
	
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

	/**
	 * Boost Attribute
	 */
	
	GameplayTags.Attributes_Boost_HealthBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.HealthBoost"),
		FString("MaxHealth Boost")
	);

	GameplayTags.Attributes_Boost_StaminaBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.StaminaBoost"),
		FString("MaxStamina Boost")
	);

	/**
	 * Character Status
	 */

	GameplayTags.Status_Action = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Action"),
		FString("Action Status")
	);

	/**
	 * Input Tag
	 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag Mouse Left")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag Mouse Right")
	);
	
    GameplayTags.InputTag_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.Right"),
        FString("Input Tag Keyboard Right")
    );

	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Space"),
		FString("Input Tag Keyboard Space")
	);


	/**
	 * Event Tag
	 */

	GameplayTags.Event_Montage_WaitInput = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.WaitInput"),
		FString("Wait InputTag From ASC")
	);
	
	GameplayTags.Event_Montage_NextAction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.NextAction"),
		FString("Event Next Action")
	);

	GameplayTags.Event_Montage_SwapWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.SwapWeapon"),
		FString("Event SwapWeapon")
	);

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Action")
	);
	
	GameplayTags.bInit = true;
}
