// Super Copyright


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	int32 EffectiveAdditionalShardAmount = FMath::Max(AdditionalShardsAmount, Level / 2);
	float EffectiveSpawnRadius = FMath::Min(AdditionalShardsSpawnRadius, 200.f + Level * 10.f);
	FString Description = FString::Printf(TEXT("<Title>神秘碎片</> <Level>Lv. %d</>\n\
<SubTitle>紫水晶: 最有用的一集</>\n\
		<Default>在选取的位置生成 1 个紫水晶碎片, 击飞紫水晶碎片内的所有目标并造成至多</> <Damage>%.1f</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"), 
		Level,

		Damage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		Cooldown,
		ManaCost

		);
	if (EffectiveAdditionalShardAmount > 0)
	{
		Description = FString::Printf(TEXT("<Title>神秘碎片</> <Level>Lv. %d</>\n\
<SubTitle>紫水晶: 最有用的一集</>\n\
		<Default>在选取的位置以及 %.0f 范围内的随机位置生成共 %d 个紫水晶碎片, 击飞紫水晶碎片内的所有目标并造成至多</> <Damage>%.1f</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
			Level,

			EffectiveSpawnRadius, 1 + EffectiveAdditionalShardAmount, Damage,
			RadialDamageInnerRadius, RadialDamageOuterRadius,
			Cooldown,
			ManaCost

		);
	}
	return Description;
}

FString UAuraArcaneShards::GetDescriptionNextLevel(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	int32 EffectiveAdditionalShardAmount = FMath::Max(AdditionalShardsAmount, Level / 2);
	float EffectiveSpawnRadius = FMath::Min(AdditionalShardsSpawnRadius, 200.f + Level * 10.f);
	FString Description = FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>紫水晶: 最有用的一集</>\n\
		<Default>在选取的位置生成 1 个紫水晶碎片, 击飞紫水晶碎片内的所有目标并造成至多</> <Damage>%.1f</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
		Level,

		Damage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		Cooldown,
		ManaCost

	);
	if (EffectiveAdditionalShardAmount > 0)
	{
		Description = FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>紫水晶: 最有用的一集</>\n\
		<Default>在选取的位置以及 %.0f 范围内的随机位置生成共 %d 个紫水晶碎片, 击飞紫水晶碎片内的所有目标并造成至多</> <Damage>%.1f</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
			Level,

			EffectiveSpawnRadius, 1 + EffectiveAdditionalShardAmount, Damage,
			RadialDamageInnerRadius, RadialDamageOuterRadius,
			Cooldown,
			ManaCost

		);
	}
	return Description;
}
