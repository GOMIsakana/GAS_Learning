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

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), TEXT("即将获得的经验。"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), TEXT("鼠标左键的输入Tag"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), TEXT("鼠标右键的输入Tag"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), TEXT("左边键盘的数字1的输入Tag"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), TEXT("左边键盘的数字2的输入Tag"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), TEXT("左边键盘的数字3的输入Tag"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), TEXT("左边键盘的数字4的输入Tag"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), TEXT("被动1的输入Tag, 仅用于识别技能位置"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), TEXT("被动2的输入Tag, 仅用于识别技能位置"));

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), TEXT("无技能"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), TEXT("攻击技能"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), TEXT("召唤技能"));
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), TEXT("受击硬直技能"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), TEXT("上锁的技能"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), TEXT("可解锁的技能"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), TEXT("已解锁的技能"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), TEXT("已装备的技能"));

	GameplayTags.Abilities_Type_Offsensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), TEXT("主动型技能"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), TEXT("被动型技能"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), TEXT("无分类技能"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), TEXT("火焰箭技能"));
	GameplayTags.Abilities_Lighting_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lighting.Electrocute"), TEXT("电击技能"));
	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), TEXT("符文碎片技能"));

	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), TEXT("火焰箭技能冷却"));
	GameplayTags.Cooldown_Lighting_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lighting.Electrocute"), TEXT("电击技能冷却"));
	GameplayTags.Cooldown_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Arcane.ArcaneShards"), TEXT("符文碎片技能冷却"));

	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HaloOfProtection"), TEXT("守护光环被动技能"));
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.LifeSiphon"), TEXT("生命虹吸被动技能"));
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaSiphon"), TEXT("法力虹吸被动技能"));

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), TEXT("使用武器攻击的插槽"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), TEXT("使用左手攻击的插槽"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), TEXT("使用右手攻击的插槽"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), TEXT("使用尾巴攻击的插槽"));

	GameplayTags.Debuff_Data_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Data.Chance"), TEXT("触发debuff的概率"));
	GameplayTags.Debuff_Data_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Data.Damage"), TEXT("触发debuff时每跳造成的伤害"));
	GameplayTags.Debuff_Data_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Data.Duration"), TEXT("debuff持续时间"));
	GameplayTags.Debuff_Data_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Data.Frequency"), TEXT("debuff跳字间隔"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), TEXT("即将到来的伤害"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), TEXT("即将到来的火焰伤害"));
	GameplayTags.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"), TEXT("即将到来的雷电伤害"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), TEXT("即将到来的神秘伤害"));

	GameplayTags.Attributes_Resist_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.FireResist"), TEXT("火焰伤害抗性"));
	GameplayTags.Attributes_Resist_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.LightingResist"), TEXT("雷电伤害抗性"));
	GameplayTags.Attributes_Resist_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.ArcaneResist"), TEXT("神秘伤害抗性"));
	GameplayTags.DamageTypeToResist.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resist_Fire);
	GameplayTags.DamageTypeToResist.Add(GameplayTags.Damage_Lighting, GameplayTags.Attributes_Resist_Lighting);
	GameplayTags.DamageTypeToResist.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resist_Arcane);

	GameplayTags.Debuff_Type_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Type.Burn"), TEXT("燃烧debuff"));
	GameplayTags.Debuff_Type_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Type.Stun"), TEXT("眩晕debuff"));
	GameplayTags.Debuff_Type_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Type.Arcane"), TEXT("神秘类型的debuff"));
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Type_Burn);
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Lighting, GameplayTags.Debuff_Type_Stun);
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Type_Arcane);

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), TEXT("攻击动作标签"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), TEXT("攻击动作标签"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), TEXT("攻击动作标签"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), TEXT("攻击动作标签"));

	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), TEXT("禁用鼠标射线检测"));
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), TEXT("禁用按下按键的交互"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), TEXT("禁用按住按键的交互"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), TEXT("禁用松开按键的交互"));
}
