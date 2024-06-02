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

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;

	FGameplayTag Abilities_Attack;

	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_LeftHand;
	FGameplayTag Montage_Attack_RightHand;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToResist;

	FGameplayTag Effects_HitReact;
protected:

private:
	// 这个的意义是为了让上面的Get有意义，上面的Get一般会以 FAuraGameplayTags::Get() 的形式调用，此时获取到的就是这个在结构体内创建的变量
	static FAuraGameplayTags GameplayTags;
};
