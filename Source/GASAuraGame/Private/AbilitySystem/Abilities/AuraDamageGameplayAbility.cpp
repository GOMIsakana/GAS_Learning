// Super Copyright


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypeMap)
	{
		float ScalableDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScalableDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAuraDamageGameplayAbility::GetDamage(FGameplayTag DamageType, float InLevel) const
{
	float Damage = 0.f;
	if (DamageTypeMap.Contains(DamageType))
	{
		Damage = DamageTypeMap[DamageType].GetValueAtLevel(InLevel);
	}
	return Damage;
}