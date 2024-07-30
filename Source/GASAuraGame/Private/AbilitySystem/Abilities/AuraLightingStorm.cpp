// Super Copyright


#include "AbilitySystem/Abilities/AuraLightingStorm.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraLightingStorm::GetDescription(int32 Level)
{
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("LightingStorm.Hit"), Level);
	float EffectiveDamageInnerRadius = bDamageRadiusOverrideByLevel ? RadialDamageOuterRadius + Level * AdditionalRadiusPerLevel : OverrideRadius;
	float EffectiveDamageOuterRadius = bDamageRadiusOverrideByLevel ? RadialDamageOuterRadius + Level * AdditionalRadiusPerLevel : OverrideRadius;

	return FString::Printf(TEXT("<Title>雷电风暴</> <Level>Lv. %d</>\n\
<SubTitle>何方道友在此渡劫?</>\n\
		<Default>在目标位置处召唤一道雷电, 对目标点周围敌人造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>电属性伤害</>\n\
<SubTitle>伤害从中心点 %0.f 范围处开始衰减, 于 %.0f 范围处衰减至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>%.1f%% 最大伤害</>\
		"),
		Level,

		ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage,
		EffectiveDamageInnerRadius, EffectiveDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level),
		MinRadialDamagePercentage
	);
}

FString UAuraLightingStorm::GetDescriptionNextLevel(int32 Level)
{
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("LightingStorm.Hit"), Level);
	float EffectiveDamageInnerRadius = bDamageRadiusOverrideByLevel ? RadialDamageOuterRadius + Level * AdditionalRadiusPerLevel : OverrideRadius;
	float EffectiveDamageOuterRadius = bDamageRadiusOverrideByLevel ? RadialDamageOuterRadius + Level * AdditionalRadiusPerLevel : OverrideRadius;

	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>何方道友在此渡劫?</>\n\
		<Default>在目标位置处召唤一道雷电, 对目标点周围敌人造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>电属性伤害</>\n\
<SubTitle>伤害从中心点 %0.f 范围处开始衰减, 于 %.0f 范围处衰减至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>%.1f%% 最大伤害</>\
		"),
		Level,

		ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage,
		EffectiveDamageInnerRadius, EffectiveDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level),
		MinRadialDamagePercentage
	);
}
