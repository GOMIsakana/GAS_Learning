// Super Copyright


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	float Damage = ScalableDamage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, Damage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAuraDamageGameplayAbility::GetDamage(FGameplayTag InDamageType, float InLevel) const
{
	float Damage = 0.f;
	if (InDamageType.MatchesTagExact(DamageType))
	{
		Damage = ScalableDamage.GetValueAtLevel(InLevel);
	}
	return Damage;
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamasFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContext = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = ScalableDamage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackMagnitude = KnockbackMagnitude;
	if (TargetActor)
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackImpulse = ToTarget * KnockbackMagnitude;
	}

	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return ScalableDamage.GetValueAtLevel(GetAbilityLevel());
}
