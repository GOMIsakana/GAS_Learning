// Super Copyright


#include "AbilitySystem/Abilities/AuraBallLighting.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraLightingBall.h"
#include "GameplayCueFunctionLibrary.h"

FString UAuraBallLighting::GetDescription(int32 Level)
{
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, int32(GetAbilityLevel() / 5)) : NumProjectile;
	return FString::Printf(TEXT("<Title>球状闪电</> <Level>Lv. %d</>\n\
<SubTitle>能随便电人的球, 电费也不贵</>\n\
		<Default>朝目标位置发射 %d 枚闪电球, 闪电球会自动寻找并连接 %.0f 范围内的至多 %d 名敌人, 且敌人与闪电球的距离超过 %.0f 时会断开连接。</>\
<Default>被闪电球连接的敌人每 %.1f 秒会受到</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>电属性伤害</><Default>, </>\
<Default>并有 %.2f%% 概率</><LightingDamage>击晕</><Default>目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\
		"),
		Level,

		EffectiveProjectileNum, TargetSearchingDistance, MaxTargetAmount, MaxConnectingDistance,
		DealDamageFrequency, ScalableDamage.GetValueAtLevel(Level), GetAdditionalAttributeValueByName(FName("BallLighting.Hit"), Level),
		DebuffChance, DebuffDuration,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level)
	);
}

FString UAuraBallLighting::GetDescriptionNextLevel(int32 Level)
{
	int32 EffectiveProjectileNum = bOverrideNumProjectileByLevel ? FMath::Max(1, int32(GetAbilityLevel() / 5)) : NumProjectile;
	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>能随便电人的球, 电费也不贵</>\n\
		<Default>朝目标位置发射 %d 枚闪电球, 闪电球会自动寻找并连接 %.0f 范围内的至多 %d 名敌人, 且敌人与闪电球的距离超过 %.0f 时会断开连接。</>\
<Default>被闪电球连接的敌人每 %.1f 秒会受到</> <Damage>%.1f + %.1f%%物理伤害</> <Default>点</><LightingDamage>电属性伤害</><Default>, </>\
<Default>并有 %.2f%% 概率</><LightingDamage>击晕</><Default>目标 %.2f 秒</>\n\n\
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n\
<Default>法力消耗:</> <ManaCost>%.2f</>\
		"),
		Level,

		EffectiveProjectileNum, TargetSearchingDistance, MaxTargetAmount, MaxConnectingDistance,
		DealDamageFrequency, ScalableDamage.GetValueAtLevel(Level), GetAdditionalAttributeValueByName(FName("BallLighting.Hit"), Level),
		DebuffChance, DebuffDuration,
		GetCooldown(Level),
		GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level)
	);
}

void UAuraBallLighting::SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (!LightingBallClass) return;

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

		AAuraLightingBall* LightingBall = GetWorld()->SpawnActorDeferred<AAuraLightingBall>(
			LightingBallClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);


		LightingBall->DealDamageFrequency = DealDamageFrequency;
		LightingBall->MaxTargetAmount = MaxTargetAmount;
		LightingBall->MaxConnectingDistance = MaxConnectingDistance;
		LightingBall->TargetSearchingDistance = TargetSearchingDistance;

		LightingBall->SetOwner(GetAvatarActorFromActorInfo());

		SetupDamageEffectParamsForProjectile(LightingBall);

		LightingBall->FinishSpawning(SpawnTransform);

		LightingBall->SetLifeSpan(LightingBallLifeSpan);

	}
	OnProjectileSpawnFinishedSignature.Broadcast();
}

void UAuraBallLighting::SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile)
{
	FDamageEffectParams ProjectileEffectParams = MakeDamageEffectParamasFromClassDefaults();
	ProjectileEffectParams.BaseDamage = 
		ProjectileEffectParams.BaseDamage + 
		GetAdditionalAttributeValueByName(FName("BallLighting.Hit"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetPhysicalDamageAttribute());
	InProjectile->DamageEffectParams = ProjectileEffectParams;
}
