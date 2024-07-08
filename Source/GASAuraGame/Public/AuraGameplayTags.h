// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura Gameplay Tags，
 * 
 * 
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_PhysicalDamage;
	FGameplayTag Attributes_Secondary_Defense;
	FGameplayTag Attributes_Secondary_DefensePenetration;
	FGameplayTag Attributes_Secondary_MagicalDamage;
	FGameplayTag Attributes_Secondary_Resist;
	FGameplayTag Attributes_Secondary_ResistPenetration;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_CritChance;
	FGameplayTag Attributes_Secondary_CritDamage;

	FGameplayTag Attributes_Resist_Fire;
	FGameplayTag Attributes_Resist_Lighting;
	FGameplayTag Attributes_Resist_Arcane;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;

	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;

	FGameplayTag Debuff_Type_Burn;
	FGameplayTag Debuff_Type_Stun;
	FGameplayTag Debuff_Type_Arcane;

	FGameplayTag Debuff_Data_Chance;
	FGameplayTag Debuff_Data_Damage;
	FGameplayTag Debuff_Data_Duration;
	FGameplayTag Debuff_Data_Frequency;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offsensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Lighting_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;

	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_FireBlast;
	FGameplayTag Cooldown_Lighting_Electrocute;
	FGameplayTag Cooldown_Arcane_ArcaneShards;

	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FGameplayTag Player_Block_CursorTrace;
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToResist;
	TMap<FGameplayTag, FGameplayTag> DamageTypeToDebuff;

protected:

private:
	// 这个的意义是为了让上面的Get有意义，上面的Get一般会以 FAuraGameplayTags::Get() 的形式调用，此时获取到的就是这个在结构体内创建的变量
	static FAuraGameplayTags GameplayTags;
};
