// Super Copyright


#include "AbilitySystem/Abilities/AuraArcaneStorm.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraArcaneCrystalProjectile.h"

FString UAuraArcaneStorm::GetDescription(int32 Level)
{
	float EffectiveCrystalLifeSpan = bOverrideCrystalLifeSpanByLevel ? CrystalLifeSpan + Level * AdditionalCrystalLifeSpanPerLevel : CrystalLifeSpan;
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Physical"), Level);
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Magical"), Level);

	return FString::Printf(TEXT("<Title>符文风暴</> <Level>Lv. %d</>\n\
<SubTitle>这不是我们无尽狂潮的回音蝠刃吗?</>\n\
		<Default>朝自身四周散射 %d 枚持续 %.1f 秒的符文结晶, 符文结晶在命中敌人或者墙壁时不会消失, </>\n\
<Default>而会造成</> <Damage>%.1f + %.1f%%物理伤害 + %.1f%%魔法伤害</> <Default>点伤害并使其伤害提升 %.1f%%</>\n\
<SubTitle>符文结晶的命中次数、弹射次数以及伤害提升的次数无上限, 伤害提升的方式为在当前伤害上直接乘算</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\
		"),
		Level,

		NumProjectile, EffectiveCrystalLifeSpan,
		ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage, AdditionalMagicalDamage, OverlapIncreaseDamageMagnitude * 100.f,

		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level)
	);
}

FString UAuraArcaneStorm::GetDescriptionNextLevel(int32 Level)
{
	float EffectiveCrystalLifeSpan = bOverrideCrystalLifeSpanByLevel ? CrystalLifeSpan + Level * AdditionalCrystalLifeSpanPerLevel : CrystalLifeSpan;
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Physical"), Level);
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Magical"), Level);

	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>这不是我们无尽狂潮的回音蝠刃吗?</>\n\
		<Default>朝自身四周散射 %d 枚持续 %.1f 秒的符文结晶, 符文结晶在命中敌人或者墙壁时不会消失, </>\n\
<Default>而会造成</> <Damage>%.1f + %.1f%%物理伤害 + %.1f%%魔法伤害</> <Default>点伤害并使其伤害提升 %.1f%%</>\n\
<SubTitle>符文结晶的命中次数、弹射次数以及伤害提升的次数无上限, 伤害提升的方式为在当前伤害上直接乘算</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\
		"),
		Level,

		NumProjectile, EffectiveCrystalLifeSpan,
		ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage, AdditionalMagicalDamage, OverlapIncreaseDamageMagnitude * 100.f,

		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level)
	);
}

void UAuraArcaneStorm::SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (!ProjectileClass) return;

	FVector SocketLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	SocketLocation.Z += 10.f;
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	}

	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	const FVector Forward = Rotation.Vector();
	int32 EffectiveProjectileNum = NumProjectile;
	float EffectiveCrystalLifeSpan = bOverrideCrystalLifeSpanByLevel ? CrystalLifeSpan + GetAbilityLevel() * AdditionalCrystalLifeSpanPerLevel : CrystalLifeSpan;
	TArray<FRotator> ProjectileSpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotator(Forward, FVector::UpVector, ProjectileSpread, EffectiveProjectileNum);
	for (FRotator SpawnRots : ProjectileSpawnRotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		if (bOverridePitch)
		{
			SpawnRots.Pitch = PitchOverride;
		}
		SpawnTransform.SetRotation(SpawnRots.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (AAuraArcaneCrystalProjectile* ArcaneCrystal = Cast<AAuraArcaneCrystalProjectile>(Projectile))
		{
			ArcaneCrystal->OverlapIncreaseDamageMagnitude = OverlapIncreaseDamageMagnitude;
		}

		SetupDamageEffectParamsForProjectile(Projectile);

		Projectile->FinishSpawning(SpawnTransform);

		Projectile->SetLifeSpan(EffectiveCrystalLifeSpan);
	}
	OnProjectileSpawnFinishedSignature.Broadcast();
}

void UAuraArcaneStorm::SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile)
{
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Physical"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetPhysicalDamageAttribute());
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneStorm.Hit.Magical"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute());
	FDamageEffectParams Params = MakeDamageEffectParamasFromClassDefaults();
	Params.BaseDamage = Params.BaseDamage + AdditionalPhysicalDamage + AdditionalMagicalDamage;
	InProjectile->DamageEffectParams = Params;
}
