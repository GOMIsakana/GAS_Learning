// Super Copyright


#include "AbilitySystem/Abilities/AuraFireNado.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UAuraFireNado::SpawnTornado(FVector SpawnLocation)
{
	if (TornadoClass)
	{
		int32 EffectiveSpawnAmount = FMath::Max(TornadoAmount, GetAbilityLevel() / 3);
		int32 EffectiveTornadoDuration = bOverrideTornadoDurationByLevel ? 5.f + GetAbilityLevel() / 2.f : TornadoDuration;
		for (int i = 0; i < EffectiveSpawnAmount; i++)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			FVector SpawnScale(RadialDamageOuterRadius / 200.f);
			SpawnTransform.SetScale3D(SpawnScale);
			AAuraTornado* Tornado = GetWorld()->SpawnActorDeferred<AAuraTornado>(
				TornadoClass,
				SpawnTransform,
				GetAvatarActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			FDamageEffectParams Params = MakeDamageEffectParamasFromClassDefaults();
			float AdditionalMagicalHitDamage = GetAdditionalAttributeValueByName(FName("FireNado.Hit"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute());
			float AdditionalMagicalDebuffDamage = GetAdditionalAttributeValueByName(FName("FireNado.DebuffDamage"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute());
			Params.BaseDamage = Params.BaseDamage + AdditionalMagicalHitDamage;
			Params.MinRadialDamage = MinRadialDamagePercentage / 100.f * Params.BaseDamage;
			Params.DebuffDamage = AdditionalMagicalDebuffDamage;
			Tornado->DamageEffectParams = Params;
			Tornado->SetLifeSpan(EffectiveTornadoDuration);
			Tornado->SetDealDamageFrequency(DealDamageFrequency);
			Tornado->SetDealingDamage(true);
			Tornado->FinishSpawning(SpawnTransform);
		}
	}
	TornadoSpawnComponentDelegate.Broadcast();
}

FString UAuraFireNado::GetDescription(int32 Level)
{
	int32 EffectiveTornadoAmount = FMath::Max(TornadoAmount, Level / 3);
	float EffectiveTornadoDuration = bOverrideTornadoDurationByLevel ? 5.f + Level / 2.f : TornadoDuration;
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float AdditionalMagicalHitDamage = GetAdditionalAttributeValueByName(FName("FireNado.Hit"), Level);
	float AdditionalMagicalDebuffDamage = GetAdditionalAttributeValueByName(FName("FireNado.DebuffDamage"), Level);
	return FString::Printf(TEXT("<Title>火龙卷</> <Level>Lv. %d</>\n\
		<SubTitle>将一切烧毁在火龙卷之中吧</>\n\
		<Default>在选取的位置处生成 %d 个持续 %.1f 秒的火龙卷, 火龙卷会自己寻找并接近距离自己最近的敌人作为目标。</>\n\
		<Default>火龙卷每 %.2f 秒对附近的敌人造成</> <Damage>%.1f + %.1f%%法术伤害</> <Default>点伤害, </>\
<Default>命中时有 %.2f%% 概率使目标</><FireDamage>燃烧</><Default> %.2f 秒, 每 %.2f 秒造成</> <Damage>%.1f%%法术伤害</> <Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火龙卷造成的范围伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</><Default>秒</>\n\
<Default>施法消耗:</> <ManaCost>%.2f</><Default>法力值 + </><HealthCost>%.2f</><Default>生命值</>\n\
<Default>最低伤害:</> <Damage>%1.f%%最大伤害</>\
		"),
		Level,

		EffectiveTornadoAmount, EffectiveTornadoDuration,
		DealDamageFrequency, Damage, AdditionalMagicalHitDamage,
		DebuffChance, DebuffDuration, DebuffFrequency, AdditionalMagicalDebuffDamage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level), GetSpellCost(UAuraAttributeSet::GetHealthAttribute(), Level),
		MinRadialDamagePercentage
	);
}

FString UAuraFireNado::GetDescriptionNextLevel(int32 Level)
{
	int32 EffectiveTornadoAmount = FMath::Max(TornadoAmount, Level / 3);
	float EffectiveTornadoDuration = bOverrideTornadoDurationByLevel ? 5.f + Level / 2.f : TornadoDuration;
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float AdditionalMagicalHitDamage = GetAdditionalAttributeValueByName(FName("FireNado.Hit"), Level);
	float AdditionalMagicalDebuffDamage = GetAdditionalAttributeValueByName(FName("FireNado.DebuffDamage"), Level);
	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
		<SubTitle>将一切烧毁在火龙卷之中吧</>\n\
		<Default>在选取的位置处生成 %d 个持续 %.1f 秒的火龙卷, 火龙卷会自己寻找并接近距离自己最近的敌人作为目标。</>\n\
		<Default>火龙卷每 %.2f 秒对附近的敌人造成</> <Damage>%.1f + %.1f%%法术伤害</> <Default>点伤害, </>\
<Default>命中时有 %.2f%% 概率使目标</><FireDamage>燃烧</><Default> %.2f 秒, 每 %.2f 秒造成</> <Damage>%.1f%%法术伤害</> <Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火龙卷造成的范围伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</><Default>秒</>\n\
<Default>施法消耗:</> <ManaCost>%.2f</><Default>法力值 + </><HealthCost>%.2f</><Default>生命值</>\n\
<Default>最低伤害:</> <Damage>%1.f%%最大伤害</>\
		"),
		Level,

		EffectiveTornadoAmount, EffectiveTornadoDuration,
		DealDamageFrequency, Damage, AdditionalMagicalHitDamage,
		DebuffChance, DebuffDuration, DebuffFrequency, AdditionalMagicalDebuffDamage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level), GetSpellCost(UAuraAttributeSet::GetHealthAttribute(), Level),
		MinRadialDamagePercentage
	);
}
