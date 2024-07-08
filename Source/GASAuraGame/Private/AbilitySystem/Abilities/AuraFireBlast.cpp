// Super Copyright


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	FString Description = FString::Printf(TEXT("<Title>碎裂火焰</> <Level>Lv. %d</>\n\
<SubTitle>爆！爆！爆！爆！！！</>\n\
		<Default>以自身为中心生成 %d 枚火焰箭向四周飞去, 火焰箭命中时会在命中位置引爆, </>\
<Default>若飞出的火焰箭未命中任何目标, 则会在一段时间后飞回身边并引爆。</>\n\
		<Default>火焰箭引爆时对该位置周围的所有目标造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>, \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火焰箭引爆的伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
		Level,

		NumOfProjectile,

		Damage,
		DebuffChance, DebuffDuration, DebuffFrequency, DebuffDamage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		Cooldown,
		ManaCost

	);
	return Description;
}

FString UAuraFireBlast::GetDescriptionNextLevel(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	FString Description = FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>爆！爆！爆！爆！！！</>\n\
		<Default>以自身为中心生成 %d 枚火焰箭向四周飞去, 火焰箭命中时会在命中位置引爆, </>\
<Default>若飞出的火焰箭未命中任何目标, 则会在一段时间后飞回身边并引爆。</>\n\
		<Default>火焰箭引爆时对该位置周围的所有目标造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>, \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火焰箭引爆的伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
		Level,

		NumOfProjectile,

		Damage,
		DebuffChance, DebuffDuration, DebuffFrequency, DebuffDamage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		Cooldown,
		ManaCost

	);
	return Description;
}