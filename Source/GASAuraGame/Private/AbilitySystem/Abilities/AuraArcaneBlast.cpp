// Super Copyright


#include "AbilitySystem/Abilities/AuraArcaneBlast.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraArcaneGrenade.h"

FString UAuraArcaneBlast::GetDescription(int32 Level)
{
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, Level / 5) : NumProjectile;
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Physical"), GetAbilityLevel());
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Magical"), GetAbilityLevel());

	return FString::Printf(TEXT("<Title>符文弹球</> <Level>Lv. %d</>\n\
<SubTitle>说是弹球, 但怎么还会Cos破片手雷?!</>\n\
		<Default>朝目标方向召唤 %d 个符文弹球, 符文弹球在弹跳时会对周围敌人造成</> <Damage>%.1f + %.1f%%物理伤害 + %1.f%%魔法伤害</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>符文弹球至多进行 %d 次弹跳, 弹跳次数归零时自动摧毁</>\n\
<SubTitle>伤害从中心点处 %.0f 范围开始衰减, 于 %.0f 范围处衰减至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>%.1f%% 最大伤害</>\
		"),
		Level,

		EffectiveProjectileNum, ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage, AdditionalMagicalDamage,
		MaxBounceTimes,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level),
		MinRadialDamagePercentage
	);
}

FString UAuraArcaneBlast::GetDescriptionNextLevel(int32 Level)
{
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, Level / 5) : NumProjectile;
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Physical"), GetAbilityLevel());
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Magical"), GetAbilityLevel());

	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>说是弹球, 但怎么还会Cos破片手雷?!</>\n\
		<Default>朝目标方向召唤 %d 个符文弹球, 符文弹球在弹跳时会对周围敌人造成</> <Damage>%.1f + %.1f%%物理伤害 + %1.f%%魔法伤害</> <Default>点</><ArcaneDamage>神秘属性伤害</>\n\
<SubTitle>符文弹球至多进行 %d 次弹跳, 弹跳次数归零时自动摧毁</>\n\
<SubTitle>伤害从中心点处 %.0f 范围开始衰减, 于 %.0f 范围处衰减至最低伤害</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\n\
<Default>最低伤害:</> <Damage>%.1f%% 最大伤害</>\
		"),
		Level,

		EffectiveProjectileNum, ScalableDamage.GetValueAtLevel(Level), AdditionalPhysicalDamage, AdditionalMagicalDamage,
		MaxBounceTimes,
		RadialDamageInnerRadius, RadialDamageOuterRadius,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level),
		MinRadialDamagePercentage
	);
}

void UAuraArcaneBlast::SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
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
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, int32(GetAbilityLevel() / 5)) : NumProjectile;
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

		SetupDamageEffectParamsForProjectile(Projectile);
		if (AAuraArcaneGrenade* ArcaneGrenade = Cast<AAuraArcaneGrenade>(Projectile))
		{
			ArcaneGrenade->RemainBounceTimes = MaxBounceTimes;
		}

		Projectile->FinishSpawning(SpawnTransform);
	}
	OnProjectileSpawnFinishedSignature.Broadcast();
}

void UAuraArcaneBlast::SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile)
{
	FDamageEffectParams Params = MakeDamageEffectParamasFromClassDefaults();
	float AdditionalPhysicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Physical"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetPhysicalDamageAttribute());
	float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("ArcaneBlast.Hit.Magical"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute());
	Params.BaseDamage = Params.BaseDamage + AdditionalPhysicalDamage + AdditionalMagicalDamage;
	Params.MinRadialDamage = MinRadialDamagePercentage / 100.f * Params.BaseDamage;
	InProjectile->DamageEffectParams = Params;
}
