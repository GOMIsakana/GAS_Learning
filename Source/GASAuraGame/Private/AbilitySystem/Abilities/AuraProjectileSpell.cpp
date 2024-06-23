// Super Copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Actor/AuraProjectile.h"
#include "GASAuraGame/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag)
{
	if (!(GetAvatarActorFromActorInfo() && GetAvatarActorFromActorInfo()->HasAuthority())) return;

	if (ProjectileClass)
	{
		for (int i = 0; i < NumProjectile; i++)
		{
			FTransform SpawnTransform;
			FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
			FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
			// Rotation.Pitch = 0.f;

			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);


			if (DamageEffectClass)
			{
				const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
				const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

				for (auto& Pair : DamageTypeMap)
				{
					const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
					UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
				}

				Projectile->DamageEffectHandle = SpecHandle;
			}

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const float Damage = DamageTypeMap[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	// 冷却时间
	FGameplayEffectSpec CooldownEffectSpec;
	float CooldownTime = 0.f;
	GetCooldownGameplayEffect()->DurationMagnitude.AttemptCalculateMagnitude(CooldownEffectSpec, CooldownTime);

	// 法力消耗 // 目前随着等级提升显示不正确，需要修正！

	float ManaCost = 0.f;
	for (FGameplayModifierInfo ModifierInfo : GetCostGameplayEffect()->Modifiers)
	{
		if (ModifierInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
		{
			FGameplayEffectSpec EffectSpec;
			float CalculatedMagnitude = 0.f;
			ModifierInfo.ModifierMagnitude.AttemptCalculateMagnitude(EffectSpec, CalculatedMagnitude);
			ManaCost -= CalculatedMagnitude;
		}
	}
	return FString::Printf(TEXT("<Title>火焰箭</> <Level>Lv. %d</>\n \
		<Default>朝目标发射一枚火焰箭, 命中时造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>\n \
		<Default>冷却时间:</> <Cooldown>%.2f</> 秒\n \
		<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), Level, Damage, CooldownTime, ManaCost);
}

FString UAuraProjectileSpell::GetDescriptionNextLevel(int32 Level)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const float Damage = DamageTypeMap[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	// 冷却时间
	FGameplayEffectSpec CooldownEffectSpec;
	float CooldownTime = 0.f;
	GetCooldownGameplayEffect()->DurationMagnitude.AttemptCalculateMagnitude(CooldownEffectSpec, CooldownTime);
	// 法力消耗
	float ManaCost = 0.f;
	for (FGameplayModifierInfo ModifierInfo : GetCostGameplayEffect()->Modifiers)
	{
		if (ModifierInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
		{
			FGameplayEffectSpec EffectSpec;
			float CalculatedMagnitude = 0.f;
			ModifierInfo.ModifierMagnitude.AttemptCalculateMagnitude(EffectSpec, CalculatedMagnitude);
			ManaCost -= CalculatedMagnitude;
		}
	}
	return FString::Printf(TEXT("<Title>火焰箭</> <Level>Lv. %d</>\n \
		<Default>朝目标发射一枚火焰箭, 命中时造成</> <Damage>%.1f</> <Default>点</><FireDamage>火属性伤害</>\n \
		<Default>冷却时间:</> <Cooldown>%.2f</> 秒\n \
		<Default>法力消耗:</> <ManaCost>%.2f</> \n \
		"), Level, Damage, CooldownTime, ManaCost);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	
	if (!bIsServer) return;
}
