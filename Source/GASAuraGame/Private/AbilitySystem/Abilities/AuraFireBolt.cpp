// Super Copyright


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	float Damage = GetDamage(GameplayTags.Damage_Fire, Level);
	float CooldownTime = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);

	return FString::Printf(TEXT("<Title>火焰箭</> <Level>Lv. %d</>\n \
		<SubTitle>弓箭手, 放箭 !!!</>\n \
		<Default>朝目标发射一枚火焰箭, 命中时造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>\n \
		<Default>冷却时间:</> <Cooldown>%.2f</> 秒\n \
		<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), Level, Damage, CooldownTime, ManaCost);
}

FString UAuraFireBolt::GetDescriptionNextLevel(int32 Level)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	float Damage = GetDamage(GameplayTags.Damage_Fire, Level);
	float CooldownTime = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);

	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n \
		<SubTitle>弓箭手, 放箭 !!!</>\n \
		<Default>朝目标发射一枚火焰箭, 命中时造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>\n \
		<Default>冷却时间:</> <Cooldown>%.2f</> 秒\n \
		<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), Level, Damage, CooldownTime, ManaCost);
}
