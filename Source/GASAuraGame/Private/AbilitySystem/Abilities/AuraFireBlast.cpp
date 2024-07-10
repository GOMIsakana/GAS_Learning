// Super Copyright


#include "AbilitySystem/Abilities/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	float Damage = ScalableDamage.GetValueAtLevel(Level);
	float Cooldown = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	FString Description = FString::Printf(TEXT("<Title>碎裂火焰</> <Level>Lv. %d</>\n\
<SubTitle>爆！爆！爆！爆！！！</>\n\
		<Default>以自身为中心生成 %d 枚火球向四周飞去, 并在一段时间后回到自身身边并引爆。</>\n\
		<Default>火球命中目标时不会引爆, 而会造成</> <Damage>%.2f</> <Default>点</><FireDamage>火属性伤害</><Default>并继续飞行</>\n\
		<Default>火球引爆时对该位置周围的所有目标造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>, \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火球引爆的伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
		Level,

		NumOfProjectile,
		Damage,
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
	FString Description = FString::Printf(TEXT("<Title>碎裂火焰</> <Level>Lv. %d</>\n\
<SubTitle>爆！爆！爆！爆！！！</>\n\
		<Default>以自身为中心生成 %d 枚火球向四周飞去, 并在一段时间后回到自身身边并引爆。</>\n\
		<Default>火球命中目标时不会引爆, 而会造成</> <Damage>%.2f</> <Default>点</><FireDamage>火属性伤害</><Default>并继续飞行</>\n\
		<Default>火球引爆时对该位置周围的所有目标造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>, \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\
<SubTitle>火球引爆的伤害从中心点的 %.0f 范围处开始衰减, 在 %.0f 范围处降至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>0.0</>\n\
		"),
		Level,

		NumOfProjectile,
		Damage,
		Damage,
		DebuffChance, DebuffDuration, DebuffFrequency, DebuffDamage,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		Cooldown,
		ManaCost

	);
	return Description;
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> SpawnedFireballs;
	const FVector OwnerForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> SpawnRotators = UAuraAbilitySystemLibrary::EvenlySpacedRotator(OwnerForwardVector, FVector::UpVector, 360.f, NumOfProjectile);
	for (const FRotator& SpawnRotator : SpawnRotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(OwnerLocation);
		SpawnTransform.SetRotation(SpawnRotator.Quaternion());

		AAuraFireBall* FireBall = GetAvatarActorFromActorInfo()->GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			GetActorInfo().PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		FireBall->SetOwner(GetAvatarActorFromActorInfo());

		FireBall->ExplodeDamageParam = MakeDamageEffectParamasFromClassDefaults();
		FDamageEffectParams OverlapDamageParam = MakeDamageEffectParamasFromClassDefaults();
		OverlapDamageParam.bIsRadialDamage = false;
		OverlapDamageParam.KnockbackChance = 0.f;
		FireBall->DamageEffectParams = OverlapDamageParam;
		FireBall->FlyBackActor = GetAvatarActorFromActorInfo();

		SpawnedFireballs.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}
	return SpawnedFireballs;
}
