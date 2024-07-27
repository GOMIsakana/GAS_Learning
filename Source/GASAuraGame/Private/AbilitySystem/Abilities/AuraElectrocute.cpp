// Super Copyright


#include "AbilitySystem/Abilities/AuraElectrocute.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraElectrocute::GetDescription(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float EndDamage = FMath::Max(0.f, Damage * 5.f);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level) / .1f;
	int32 EffectiveTargetNum = FMath::Max(NumOfAdditionalTarget, int32(Level / 3));
	float AdditionalTargetRadius = FMath::Max(AdditionalTargetSearchRadius, 350.f + Level * 50.f);

	FString Description = FString::Printf(TEXT("<Title>电击</> <Level>Lv. %d</>\n\
<SubTitle>左零右火, 雷公助我!</>\n\
<Warning>[此技能需要选中一个目标]</>\n\
		<Default>激活时, 对目标持续造成伤害, 每秒造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</>\n\
		<Default>在技能结束时, 对目标额外造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</><Default>, 并有 %.2f%% 概率</><LightingDamage>击晕</><Default>目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</> <Default>/ 秒</>\n\
		"),
		Level,
		
		
		Damage * 10.f, GetAdditionalAttributeValueByName(FName("Electrocute.Looping"), Level),
		EndDamage, GetAdditionalAttributeValueByName(FName("Electrocute.End"), Level), DebuffChance, DebuffDuration,
		Cooldown,
		ManaCost);
	if (EffectiveTargetNum > 0)
	{
		Description = FString::Printf(TEXT("<Title>电击</> <Level>Lv. %d</>\n\
<SubTitle>左零右火, 雷公助我!</>\n\
<Warning>[此技能需要选中一个目标]</>\n\
		<Default>激活时, 对目标以及目标周围 %.0f 范围内的至多 %d 名敌人持续造成伤害, 每秒造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</>\n\
		<Default>在技能结束时, 对所有目标额外造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</><Default>, 并有 %.2f%% 概率</><LightingDamage>击晕</><Default>所有目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</> <Default>/ 秒</>\n\
		"),
			Level,


			AdditionalTargetRadius, EffectiveTargetNum, Damage * 10.f, GetAdditionalAttributeValueByName(FName("Electrocute.Looping"), Level),
			EndDamage, GetAdditionalAttributeValueByName(FName("Electrocute.End"), Level), DebuffChance, DebuffDuration,
			Cooldown,
			ManaCost);
	}
	return Description;
}

FString UAuraElectrocute::GetDescriptionNextLevel(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float EndDamage = FMath::Max(0.f, Damage * 5.f);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level) / .1f;
	int32 EffectiveTargetNum = FMath::Max(NumOfAdditionalTarget, int32(Level / 3));
	float AdditionalTargetRadius = FMath::Max(AdditionalTargetSearchRadius, 350.f + Level * 50.f);

	FString Description = FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>左零右火, 雷公助我!</>\n\
<Warning>[此技能需要选中一个目标]</>\n\
		<Default>激活时, 对目标持续造成伤害, 每秒造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</>\n\
		<Default>在技能结束时, 对目标额外造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</><Default>, 并有 %.2f%% 概率</><LightingDamage>击晕</><Default>目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</> <Default>/ 秒</>\n\
		"),
		Level,


		Damage * 10.f, GetAdditionalAttributeValueByName(FName("Electrocute.Looping"), Level),
		EndDamage, GetAdditionalAttributeValueByName(FName("Electrocute.End"), Level), DebuffChance, DebuffDuration,
		Cooldown,
		ManaCost);
	if (EffectiveTargetNum > 0)
	{
		Description = FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>左零右火, 雷公助我!</>\n\
<Warning>[此技能需要选中一个目标]</>\n\
		<Default>激活时, 对目标以及目标周围 %.0f 范围内的至多 %d 名敌人持续造成伤害, 每秒造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</>\n\
		<Default>在技能结束时, 对所有目标额外造成</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>雷属性伤害</><Default>, 并有 %.2f%% 概率</><LightingDamage>击晕</><Default>所有目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</> <Default>/ 秒</>\n\
		"),
			Level,


			AdditionalTargetRadius, EffectiveTargetNum, Damage * 10.f, GetAdditionalAttributeValueByName(FName("Electrocute.Looping"), Level),
			EndDamage, GetAdditionalAttributeValueByName(FName("Electrocute.End"), Level), DebuffChance, DebuffDuration,
			Cooldown,
			ManaCost);
	}
	return Description;
}
