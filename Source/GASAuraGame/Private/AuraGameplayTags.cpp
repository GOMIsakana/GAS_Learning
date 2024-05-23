// Super Copyright


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), TEXT("生命。\n单位在死亡前最多可承受的伤害"));
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"), TEXT("法力。\n单位拥有的一种施法资源。"));

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), TEXT("力量。\n提供物理伤害、防御与少量生命再生"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), TEXT("智力。\n提供魔法伤害、抗性、法力上限与法力再生"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), TEXT("韧性。\n提供防御穿透、抗性穿透、暴击几率与暴击伤害"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), TEXT("活力。\n提供生命上限与生命再生"));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), TEXT("生命上限。\n单位拥有的生命值资源上限"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), TEXT("法力上限。\n单位拥有的法力上限"));
	GameplayTags.Attributes_Secondary_PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicalDamage"), TEXT("物理伤害。\n提高物理系攻击的伤害"));
	GameplayTags.Attributes_Secondary_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Defense"), TEXT("防御。\n抵消物理伤害。当防御为负时, 增加受到的物理伤害"));
	GameplayTags.Attributes_Secondary_DefensePenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.DefensePenetration"), TEXT("防御穿透。\n无视部分防御抵消的物理伤害。可在伤害结算时使目标抗性为负"));
	GameplayTags.Attributes_Secondary_MagicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicalDamage"), TEXT("魔法伤害。\n提高法术与技能的伤害"));
	GameplayTags.Attributes_Secondary_Resist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Resist"), TEXT("抗性。\n抵消魔法伤害。当抗性为负时, 增加受到的魔法伤害"));
	GameplayTags.Attributes_Secondary_ResistPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ResistPenetration"), TEXT("抗性穿透。\n无视部分抗性, 可在伤害结算时使目标抗性为负"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), TEXT("生命再生。\n每一秒自行恢复的生命的数量"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), TEXT("法力再生。\n每一秒自行恢复的法力的数量"));
	GameplayTags.Attributes_Secondary_CritChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CritChance"), TEXT("暴击几率。\n提高一种强化攻击的几率，触发加强攻击时, 伤害将会受到暴击伤害影响"));
	GameplayTags.Attributes_Secondary_CritDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CritDamage"), TEXT("暴击伤害。\n影响由暴击几率触发的强化攻击的伤害。当该值低于100%时将会减少伤害，大于100%时将会增加伤害"));

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), TEXT("鼠标左键的输入Tag"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), TEXT("鼠标右键的输入Tag"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), TEXT("左边键盘的数字1的输入Tag"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), TEXT("左边键盘的数字2的输入Tag"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), TEXT("左边键盘的数字3的输入Tag"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), TEXT("左边键盘的数字4的输入Tag"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), TEXT("即将到来的伤害"));
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), TEXT("受击硬直时给予的Tag"));
}
