// Super Copyright


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SceneComponent.h"

void UAuraFireBolt::SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
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
	int32 EffectiveProjectileNum = FMath::Max(NumProjectile, int32(GetAbilityLevel() / 3));
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
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetOwningActorFromActorInfo()), 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		
		if (DamageEffectClass)
		{
			FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
			
			float ScaledDamage = ScalableDamage.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);

			FDamageEffectParams ProjectileEffectParams = MakeDamageEffectParamasFromClassDefaults();
			float AdditionalDamage = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute()) * GetAdditionalAttributeValueByName(FName("FireBolt.Hit"), GetAbilityLevel()) / 100.f;
			SetScalableDamage(ProjectileEffectParams, GetScalableDamage(ProjectileEffectParams) + AdditionalDamage);
			Projectile->DamageEffectParams = ProjectileEffectParams;
		}

		Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		Projectile->HomingTargetSceneComponent->SetWorldLocation(TargetLocation);
		Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;

		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectile;
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);

		Projectile->FinishSpawning(SpawnTransform);
		OnProjectileSpawnFinishedSignature.Broadcast();
	}
}

FString UAuraFireBolt::GetDescription(int32 Level)
{
	// 在技能未激活时去GetASCFromActorInfo()会因为ActorInfo不存在而失败
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	float Damage = GetDamage(GameplayTags.Damage_Fire, Level);
	float CooldownTime = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	int32 EffectiveProjectileNum = FMath::Max(NumProjectile, int32(Level / 3));

	return FString::Printf(TEXT("<Title>火焰箭</> <Level>Lv. %d</>\n \
<SubTitle>弓箭手, 放箭 !!!</>\n \
		<Default>朝目标发射 %d 枚火焰箭, 命中时造成</> <Damage>%.1f + %.1f%%法术伤害</> <Default>点</><FireDamage>火属性伤害,</> \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\n \
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n \
<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), 
		Level, 

		EffectiveProjectileNum, Damage, GetAdditionalAttributeValueByName(FName("FireBolt.Hit"), Level),
		DebuffChance, DebuffDuration, DebuffFrequency, DebuffDamage,
		CooldownTime, 
		ManaCost);
}

FString UAuraFireBolt::GetDescriptionNextLevel(int32 Level)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	float Damage = GetDamage(GameplayTags.Damage_Fire, Level);
	float CooldownTime = GetCooldown(Level);
	float ManaCost = GetSpellCost(UAuraAttributeSet::GetManaAttribute(), Level);
	int32 EffectiveProjectileNum = FMath::Max(NumProjectile, int32(Level / 3));

	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n \
<SubTitle>弓箭手, 放箭 !!!</>\n \
		<Default>朝目标发射 %d 枚火焰箭, 命中时造成</> <Damage>%.1f + %.1f%%法术伤害</> <Default>点</><FireDamage>火属性伤害,</> \
<Default>并有 %.2f%% 概率使目标</><FireDamage>燃烧</> <Default>%.2f 秒, 每 %.2f 秒造成</> <Damage> %.1f </><Default>点</><FireDamage>火属性伤害</>\n\n \
<Default>冷却时间:</> <Cooldown>%.2f</> <Default>秒</>\n \
<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), 
		Level,

		EffectiveProjectileNum, Damage, GetAdditionalAttributeValueByName(FName("FireBolt.Hit"), Level),
		DebuffChance, DebuffDuration, DebuffFrequency, DebuffDamage,
		CooldownTime,
		ManaCost);
}
