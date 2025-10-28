
#include "SoulLikeGameplayTags.h"
#include "GameplayTagsManager.h"

FSoulLikeGameplayTags FSoulLikeGameplayTags::GameplayTags;

const FSoulLikeGameplayTags& FSoulLikeGameplayTags::Get()
{
	if(!GameplayTags.bInit)
	{
		InitializeNativeGameplayTags();
	}
	return GameplayTags;
}

void FSoulLikeGameplayTags::InitializeNativeGameplayTags()
{
	if(GameplayTags.bInit) return;
	
	const FGameplayTag CheckTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Vigor"), false);
	if (CheckTag.IsValid())
	{
		// 이미 태그가 등록되어 있으면 초기화 생략
		GameplayTags.bInit = true;
		return;
	}
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
	
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum Health Point")
	);
	
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum Stamina Point")
	);

	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegeneration"),
		FString("Stamina Regeneration")
	);
	
	/**
	 * Resistance Attribute
	 */
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"), 
		FString("Resistance to Physical damage")
	);
	
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"), 
		FString("Resistance to Fire damage")
	);
	
	GameplayTags.Attributes_Resistance_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Magic"), 
		FString("Resistance to Magic damage")
	);
	
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lighting"), 
		FString("Resistance to Lighting damage")
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

	GameplayTags.Attributes_Boost_DamageBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.DamageBoost"),
		FString("Damage Boost")
	);
	
	GameplayTags.Attributes_Boost_PhysicalDamageBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.PhysicalDamageBoost"),
		FString("Physical Damage Boost")
	);
	
	GameplayTags.Attributes_Boost_FireDamageBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.FireDamageBoost"),
		FString("Fire Damage Boost")
	);
	
	GameplayTags.Attributes_Boost_MagicDamageBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.MagicDamageBoost"),
		FString("Magic Damage Boost")
	);
	
	GameplayTags.Attributes_Boost_LightningDamageBoost = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Boost.LightningDamageBoost"),
		FString("Lightning Damage Boost")
	);

	/**
	 * Vital Attribute
	 */
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"),
		FString("Health Point")
	);

	GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Stamina"),
		FString("Stamina Point")
	);

	/**
	 * Character Status
	 */

	GameplayTags.Status_Action = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Action"),
		FString("Action Status")
	);

	GameplayTags.Status_Rolling = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rolling"),
		FString("Rolling Status")
	);

	GameplayTags.Status_TargetLock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.TargetLock"),
		FString("TargetLock Status")
	);

	GameplayTags.Status_Object = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Object"),
		FString("Object Status")
	);

	GameplayTags.Status_Rest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rest"),
		FString("Rest Status")
	);

	GameplayTags.Status_Ladder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Ladder"),
		FString("Ladder Status")
	);

	GameplayTags.Status_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.HitReact"),
		FString("HitReact Status")
	);

	GameplayTags.Status_Riposte = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Riposte"),
		FString("Riposte Status")
	);
	
	GameplayTags.Status_Invincibility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Invincibility"),
		FString("Invincibility Status")
	);
	
	GameplayTags.Status_Groggy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Groggy"),
		FString("Groggy Status")
	);

	GameplayTags.Status_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Death"),
		FString("Death Status")
	);

	GameplayTags.Status_SuperArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.SuperArmor"),
		FString("SuperArmor Status")
	);
	
	/**
	 * AI Status
	 */
	GameplayTags.Status_AI = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI"),
		FString("AI None Status")
	);
	
	GameplayTags.Status_AI_Idle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI.Idle"),
		FString("AI Idle Status")
	);

	GameplayTags.Status_AI_Chasing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI.Chasing"),
		FString("AI Chasing Status")
	);
	
	GameplayTags.Status_AI_Battle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI.Battle"),
		FString("AI Battle Status")
	);

	GameplayTags.Status_AI_Patrol = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI.Patrol"),
		FString("AI Patrol Status")
	);

	GameplayTags.Status_AI_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.AI.Death"),
		FString("AI Death Status")
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

	GameplayTags.InputTag_MB3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MB3"),
		FString("Input Tag Mouse Wheel Button")
	);
	
	GameplayTags.InputTag_MB4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MB4"),
		FString("Input Tag Mouse 4 Button")
	);
	
	GameplayTags.InputTag_MB5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MB5"),
		FString("Input Tag Mouse 5 Button")
	);
	
	GameplayTags.InputTag_MWU = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MWU"),
		FString("Input Tag Mouse Wheel Up")
	);
	
	GameplayTags.InputTag_MWD = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MWD"),
		FString("Input Tag Mouse Wheel Down")
	);

	GameplayTags.InputTag_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Left"),
		FString("Input Tag Keyboard Left")
	);
	
    GameplayTags.InputTag_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.Right"),
        FString("Input Tag Keyboard Right")
    );

	GameplayTags.InputTag_Up = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Up"),
		FString("Input Tag Keyboard Up")
	);

	GameplayTags.InputTag_Down = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Down"),
		FString("Input Tag Keyboard Down")
	);

	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Input Tag Keyboard Q")
	);
	
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input Tag Keyboard E")
	);
	
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("Input Tag Keyboard R")
	);

	GameplayTags.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.T"),
		FString("Input Tag Keyboard T")
	);
	
	GameplayTags.InputTag_Y = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Y"),
		FString("Input Tag Keyboard Y")
	);
	GameplayTags.InputTag_U = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.U"),
		FString("Input Tag Keyboard U")
	);
	GameplayTags.InputTag_I = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.I"),
		FString("Input Tag Keyboard I")
	);
	GameplayTags.InputTag_O = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.O"),
		FString("Input Tag Keyboard O")
	);
	GameplayTags.InputTag_P = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.P"),
		FString("Input Tag Keyboard P")
	);
	
	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.F"),
		FString("Input Tag Keyboard F")
	);
	GameplayTags.InputTag_G = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.G"),
		FString("Input Tag Keyboard G")
	);
	GameplayTags.InputTag_H = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.H"),
		FString("Input Tag Keyboard H")
	);
	GameplayTags.InputTag_J = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.J"),
		FString("Input Tag Keyboard J")
	);
	GameplayTags.InputTag_K = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.K"),
		FString("Input Tag Keyboard K")
	);
	GameplayTags.InputTag_L = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.L"),
		FString("Input Tag Keyboard L")
	);

	GameplayTags.InputTag_Z = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Z"),
		FString("Input Tag Keyboard Z")
	);
	GameplayTags.InputTag_X = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.X"),
		FString("Input Tag Keyboard X")
	);
	GameplayTags.InputTag_C = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.C"),
		FString("Input Tag Keyboard C")
	);
	GameplayTags.InputTag_V = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.V"),
		FString("Input Tag Keyboard V")
	);
	GameplayTags.InputTag_B = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.B"),
		FString("Input Tag Keyboard B")
	);
	GameplayTags.InputTag_N = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.N"),
		FString("Input Tag Keyboard N")
	);
	GameplayTags.InputTag_M = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.M"),
		FString("Input Tag Keyboard M")
	);

	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Space"),
		FString("Input Tag Keyboard Space")
	);

	GameplayTags.InputTag_Tab = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Tab"),
		FString("Input Tag Keyboard Tab")
	);
	
	GameplayTags.InputTag_ESC = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ESC"),
		FString("Input Tag Keyboard ESC")
	);

	/**
	 * UITask Task Tag
	 */
	GameplayTags.UITask_Back = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("UITask.Back"),
		FString("UI Back Task")
	);

	/**
	 * InputMode Tag
	 */
	GameplayTags.InputMode_Game = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputMode.Game"),
		FString("Game Input Mode")
	);
	
	GameplayTags.InputMode_UI = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputMode.UI"),
		FString("UI Input Mode")
	);
	
	GameplayTags.InputMode_KeyBind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputMode.KeyBind"),
		FString("KeyBind Input Mode")
	);
	
	/**
	 * Event Tag
	 */

	GameplayTags.Event_Montage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage"),
		FString("Event Montage")
	);

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
	
	GameplayTags.Event_Montage_RiposteDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.RiposteDamage"),
		FString("Event Montage RiposteDamage")
	);

	GameplayTags.Event_Montage_CheckDeath = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.CheckDeath"),
		FString("Event Montage Check Death")
	);

	GameplayTags.Event_Montage_UsingTool = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.UsingTool"),
		FString("Event Montage UsingTool")
	);

	GameplayTags.Event_Montage_Effect = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.Effect"),
		FString("Event Montage Effect")
	);


	GameplayTags.Event_Montage_Effect_Apply = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.Effect.Apply"),
		FString("Event Montage Apply Effect")
	);

	GameplayTags.Event_Montage_Effect_Remove = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Montage.Effect.Remove"),
		FString("Event Montage Apply Remove")
	);
	
	/**
	 * Ability Tag
	 */

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Action")
	);

	GameplayTags.Abilities_Stagger = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Stagger"),
		FString("Stagger Action")
	);
	
	GameplayTags.Abilities_Stagger_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Stagger.HitReact"),
		FString("HitReact Action")
	);

	GameplayTags.Abilities_Stagger_HitReact_Trigger = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Stagger.HitReact.Trigger"),
		FString("HitReact Trigger")
	);
	
	GameplayTags.Abilities_Stagger_Groggy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Stagger.Groggy"),
		FString("Groggy Action")
	);

	GameplayTags.Abilities_Riposte = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Riposte"),
		FString("Riposte Action")
	);
	
	GameplayTags.Abilities_Backstab = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Backstab"),
		FString("Backstab Action")
	);

	GameplayTags.Abilities_Rolling = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Rolling"),
		FString("Rolling Action")
	);

	GameplayTags.Abilities_UsingTool = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.UsingTool"),
		FString("Tool Using Action")
	);
	
	GameplayTags.Abilities_Interaction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Interaction"),
		FString("Interaction")
	);

	GameplayTags.Abilities_TargetLock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.TargetLock"),
		FString("TargetLock")
	);
	
	GameplayTags.Abilities_SwapEquipSlot_RightWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwapEquipSlot.RightWeapon"),
		FString("Swap EquipSlot RightWeapon")
	);
	
	GameplayTags.Abilities_SwapEquipSlot_LeftWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwapEquipSlot.LeftWeapon"),
		FString("Swap EquipSlot LeftWeapon")
	);
	
	GameplayTags.Abilities_SwapEquipSlot_Tool = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwapEquipSlot.Tool"),
		FString("Swap EquipSlot Tool")
	);
	
	GameplayTags.Abilities_SwapEquipSlot_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SwapEquipSlot.Spell"),
		FString("Swap EquipSlot Spell")
	);

	GameplayTags.AbilityType_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityType.Active"),
		FString("Active Ability Type")
	);
	
	GameplayTags.AbilityType_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityType.Passive"),
		FString("Passive Ability Type")
	);
	
	/**
	 * Interaction Tag
	 */
	GameplayTags.Interaction_Ladder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Ladder"),
		FString("Interaction Ladder")
	);

	GameplayTags.Interaction_Ladder_Up = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Ladder.Up"),
		FString("Interaction Ladder Up")
	);
	
	GameplayTags.Interaction_Ladder_Down = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Ladder.Down"),
		FString("Interaction Ladder Down")
	);
	
	GameplayTags.Interaction_Rest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Rest"),
		FString("Interaction Rest")
	);
	
	GameplayTags.Interaction_NPC = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.NPC"),
		FString("Interaction NPC")
	);
	
	GameplayTags.Interaction_Object = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Object"),
		FString("Interaction Object")
	);

	GameplayTags.Interaction_Object_Box = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Object.Box"),
		FString("Interaction Box")
	);

	GameplayTags.Interaction_Object_Lever = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Object.Lever"),
		FString("Interaction Lever")
	);

	GameplayTags.Interaction_Pickup = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.Pickup"),
		FString("Interaction Pickup Item")
	);

	GameplayTags.Interaction_SummonSign = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Interaction.SummonSign"),
		FString("Interaction Summon Sign")
	);
	
	/**
	 * Effect Tag
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("HitReact Effect")
	);
	
	/**
	 * EquipSlot Tag
	 */

	GameplayTags.EquipSlot_RightWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipSlot.RightWeapon"),
		FString("EquipSlot RightWeapon")
	);
	
	GameplayTags.EquipSlot_LeftWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipSlot.LeftWeapon"),
		FString("EquipSlot LeftWeapon")
	);
	
	GameplayTags.EquipSlot_Gear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipSlot.Gear"),
		FString("EquipSlot Gear")
	);
	
	GameplayTags.EquipSlot_Accessory = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipSlot.Accessory"),
		FString("EquipSlot Accessory")
	);
	
	GameplayTags.EquipSlot_Tool = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipSlot.Tool"),
		FString("EquipSlot Tool")
	);

	/**
	 * ItemType Tag
	 */

	GameplayTags.ItemType_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Weapon"),
		FString("ItemType Weapon")
	);

	GameplayTags.ItemType_Weapon_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Weapon.Equipped"),
		FString("ItemType Equipped Weapon")
	);
	
	GameplayTags.ItemType_Weapon_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Weapon.None"),
		FString("ItemType Weapon None")
	);
	GameplayTags.ItemType_Weapon_Katana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Weapon.Katana"),
		FString("ItemType Weapon Katana")
	);
	GameplayTags.ItemType_Weapon_Sword = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Weapon.Sword"),
		FString("ItemType Weapon Sword")
	);

	GameplayTags.ItemType_Gear = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Gear"),
		FString("ItemType Gear")
	);
	
	GameplayTags.ItemType_Gear_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Gear.None"),
		FString("ItemType Gear None")
	);

	GameplayTags.ItemType_Accessory = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Accessory"),
		FString("ItemType Accessory")
	);
	
	GameplayTags.ItemType_Accessory_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Accessory.None"),
		FString("ItemType Accessory None")
	);

	GameplayTags.ItemType_Tool = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Tool"),
		FString("ItemType Tool")
	);
	GameplayTags.ItemType_Tool_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Tool.None"),
		FString("ItemType Tool None")
	);
	GameplayTags.ItemType_Tool_Potion = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Tool.Potion"),
		FString("ItemType Tool Potion")
	);
	
	GameplayTags.ItemType_Tool_WhiteStone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Tool.WhiteStone"),
		FString("ItemType Tool WhiteStone")
	);

	GameplayTags.ItemType_Item = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Item"),
		FString("ItemType Item")
	);
	
	GameplayTags.ItemType_Item_UpgradeMaterial = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Item.UpgradeMaterial"),
		FString("ItemType Item UpgradeMaterial")
	);

	GameplayTags.ItemType_Skill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Skill"),
		FString("ItemType Skill")
	);
	
	GameplayTags.ItemType_Skill_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ItemType.Skill.None"),
		FString("ItemType Skill None")
	);

	GameplayTags.DamageType_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Physical"),
		FString("Physical DamageType")
	);
	
	GameplayTags.DamageType_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Fire"),
		FString("Fire DamageType")
	);

	GameplayTags.DamageType_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Magic"),
		FString("Magic DamageType")
	);
	
	GameplayTags.DamageType_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DamageType.Lightning"),
		FString("Lightning DamageType")
	);
	
	GameplayTags.BehaviorTree_Battle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("BehaviorTree.Battle"),
		FString("Battle BehaviorTree TagS")
	);

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.DamageType_Magic, GameplayTags.Attributes_Resistance_Magic);

	GameplayTags.DamageTypesToBoostAttribute.Add(GameplayTags.DamageType_Physical, GameplayTags.Attributes_Primary_Strength);
	GameplayTags.DamageTypesToBoostAttribute.Add(GameplayTags.DamageType_Fire, GameplayTags.Attributes_Primary_Intelligence);
	GameplayTags.DamageTypesToBoostAttribute.Add(GameplayTags.DamageType_Lightning, GameplayTags.Attributes_Primary_Dexterity);
	GameplayTags.DamageTypesToBoostAttribute.Add(GameplayTags.DamageType_Magic, GameplayTags.Attributes_Primary_Intelligence);

	GameplayTags.DamageTypesToDamageBoost.Add(GameplayTags.DamageType_Physical, GameplayTags.Attributes_Boost_PhysicalDamageBoost);
	GameplayTags.DamageTypesToDamageBoost.Add(GameplayTags.DamageType_Fire, GameplayTags.Attributes_Boost_FireDamageBoost);
	GameplayTags.DamageTypesToDamageBoost.Add(GameplayTags.DamageType_Lightning, GameplayTags.Attributes_Boost_LightningDamageBoost);
	GameplayTags.DamageTypesToDamageBoost.Add(GameplayTags.DamageType_Magic, GameplayTags.Attributes_Boost_MagicDamageBoost);
	
	
	GameplayTags.WeaponTypeIndex.Add(GameplayTags.ItemType_Weapon_None, 0);
	GameplayTags.WeaponTypeIndex.Add(GameplayTags.ItemType_Weapon_Katana, 1);
	GameplayTags.WeaponTypeIndex.Add(GameplayTags.ItemType_Weapon_Sword, 2);

	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Primary_Strength);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Primary_Dexterity);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Primary_Intelligence);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_DamageBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_PhysicalDamageBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_FireDamageBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_MagicDamageBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_LightningDamageBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_HealthBoost);
	GameplayTags.PassiveCaptureAttributes.Add(GameplayTags.Attributes_Boost_StaminaBoost);
	
	GameplayTags.bInit = true;
}
