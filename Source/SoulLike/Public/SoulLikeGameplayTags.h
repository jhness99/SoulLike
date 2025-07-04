 
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton Object
 * Native로 선언한 GameplayTag를 C++에서 접근하기 쉽게 하기 위해 사용
 */
struct FSoulLikeGameplayTags
{
public:

	static const FSoulLikeGameplayTags& Get();
	
	static void InitializeNativeGameplayTags();

	/**
	 * Primary Attribute
	 * 기본 Attribute
	 */
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Mind;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	
	/**
	 * Secondary Attribute
	 * Primary Attribute에 의해 값이 바뀌는 Attribute
	 */
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;

	/**
	 * Resistance Attribute
	 */
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Magic;
	FGameplayTag Attributes_Resistance_Lightning;
	
	/**
	 * Vital Attribute
	 */
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Stamina;
	
	/**
	 * Boost Attribute
	 */
	FGameplayTag Attributes_Boost_HealthBoost;
	FGameplayTag Attributes_Boost_StaminaBoost;
	
	FGameplayTag Attributes_Boost_DamageBoost;
	FGameplayTag Attributes_Boost_PhysicalDamageBoost;
	FGameplayTag Attributes_Boost_FireDamageBoost;
	FGameplayTag Attributes_Boost_MagicDamageBoost;
	FGameplayTag Attributes_Boost_LightningDamageBoost;
	
	/**
	 * Character Status
	 */
	FGameplayTag Status_Action;
	FGameplayTag Status_Rolling;
	FGameplayTag Status_TargetLock;
	FGameplayTag Status_Object;
	FGameplayTag Status_Rest;
	FGameplayTag Status_Ladder;
	FGameplayTag Status_HitReact;
	FGameplayTag Status_Riposte;
	FGameplayTag Status_Invincibility;
	FGameplayTag Status_Groggy;
	FGameplayTag Status_Death;
	FGameplayTag Status_SuperArmor;
	
	/**
	 * AI Status
	 */
	FGameplayTag Status_AI;
	FGameplayTag Status_AI_Idle;
	FGameplayTag Status_AI_Chasing;
	FGameplayTag Status_AI_Battle;
	FGameplayTag Status_AI_Patrol;
	FGameplayTag Status_AI_Death;	
	
	/**
	 * Input Tag
	 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_MB3;
	FGameplayTag InputTag_MB4;
	FGameplayTag InputTag_MB5;
	FGameplayTag InputTag_MWU;
	FGameplayTag InputTag_MWD;
	FGameplayTag InputTag_Left;
    FGameplayTag InputTag_Right;
	FGameplayTag InputTag_Up;
	FGameplayTag InputTag_Down;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_T;
	FGameplayTag InputTag_Y;
	FGameplayTag InputTag_U;
	FGameplayTag InputTag_I;
	FGameplayTag InputTag_O;
	FGameplayTag InputTag_P;
	
	FGameplayTag InputTag_F;
	FGameplayTag InputTag_G;
	FGameplayTag InputTag_H;
	FGameplayTag InputTag_J;
	FGameplayTag InputTag_K;
	FGameplayTag InputTag_L;

	FGameplayTag InputTag_Z;
	FGameplayTag InputTag_X;
	FGameplayTag InputTag_C;
	FGameplayTag InputTag_V;
	FGameplayTag InputTag_B;
	FGameplayTag InputTag_N;
	FGameplayTag InputTag_M;
	
	FGameplayTag InputTag_Space;

	FGameplayTag InputTag_Tab;
	FGameplayTag InputTag_ESC;

	/**
	 * UITask Task Tag
	 */
	FGameplayTag UITask_Back;

	/**
	 * Input Mode Tag
	 */
	FGameplayTag InputMode_Game;
	FGameplayTag InputMode_UI;
	FGameplayTag InputMode_KeyBind;
	
	/**
	 * Event Tag
	 */
	FGameplayTag Event_Montage;
	FGameplayTag Event_Montage_WaitInput;
	FGameplayTag Event_Montage_NextAction;
	
	FGameplayTag Event_Montage_SwapWeapon;

	FGameplayTag Event_Montage_RiposteDamage;
	FGameplayTag Event_Montage_CheckDeath;

	FGameplayTag Event_Montage_UsingTool;

	FGameplayTag Event_Montage_Effect;
	FGameplayTag Event_Montage_Effect_Apply;
	FGameplayTag Event_Montage_Effect_Remove;

	/**
	 * Ability Tag
	 */
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Stagger;
	FGameplayTag Abilities_Stagger_HitReact;
	FGameplayTag Abilities_Stagger_HitReact_Trigger;
	FGameplayTag Abilities_Stagger_Groggy;
	FGameplayTag Abilities_Riposte;
	FGameplayTag Abilities_Backstab;
	FGameplayTag Abilities_Rolling;
	FGameplayTag Abilities_UsingTool;

	FGameplayTag Abilities_Interaction;
	FGameplayTag Abilities_TargetLock;
	
	FGameplayTag Abilities_SwapEquipSlot_RightWeapon;
	FGameplayTag Abilities_SwapEquipSlot_LeftWeapon;
	FGameplayTag Abilities_SwapEquipSlot_Tool;
	FGameplayTag Abilities_SwapEquipSlot_Spell;

	FGameplayTag AbilityType_Active;
	FGameplayTag AbilityType_Passive;
	
	/**
	 * Interaction Tag
	 */
	FGameplayTag Interaction_Ladder;
	FGameplayTag Interaction_Ladder_Up;
	FGameplayTag Interaction_Ladder_Down;
	
	FGameplayTag Interaction_Rest;
	FGameplayTag Interaction_NPC;
	
	FGameplayTag Interaction_Object;
	FGameplayTag Interaction_Object_Box;
	FGameplayTag Interaction_Object_Lever;
	
	FGameplayTag Interaction_Pickup;

	/**
	 * Effect Tag
	 */
	FGameplayTag Effects_HitReact;
	
	/**
	 * EquipSlot Tag
	 */
	FGameplayTag EquipSlot_RightWeapon;
	FGameplayTag EquipSlot_LeftWeapon;
	FGameplayTag EquipSlot_Gear;
	FGameplayTag EquipSlot_Accessory;
	FGameplayTag EquipSlot_Tool;
	
	/**
	 * ItemType Tag
	 */
	FGameplayTag ItemType_Weapon;
	FGameplayTag ItemType_Weapon_None;
	FGameplayTag ItemType_Weapon_Equipped;
	FGameplayTag ItemType_Weapon_Katana;
	FGameplayTag ItemType_Weapon_Sword;

	FGameplayTag ItemType_Gear;
	FGameplayTag ItemType_Gear_None;

	FGameplayTag ItemType_Accessory;
	FGameplayTag ItemType_Accessory_None;

	FGameplayTag ItemType_Tool;
	FGameplayTag ItemType_Tool_None;
	FGameplayTag ItemType_Tool_Potion;

	FGameplayTag ItemType_Item;
	FGameplayTag ItemType_Item_UpgradeMaterial;

	FGameplayTag ItemType_Skill;
	FGameplayTag ItemType_Skill_None;

	/**
	 * Damage Tag
	 */
	FGameplayTag DamageType_Physical;
	FGameplayTag DamageType_Fire;
	FGameplayTag DamageType_Magic;
	FGameplayTag DamageType_Lightning;

	/**
	 * BehaviorTree
	 */
	FGameplayTag BehaviorTree_Battle;


	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToBoostAttribute;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDamageBoost;
	TMap<FGameplayTag, int32> WeaponTypeIndex;

	TArray<FGameplayTag> PassiveCaptureAttributes;
	
	bool bInit = false;
	
private:
	
	static FSoulLikeGameplayTags GameplayTags;

	
};