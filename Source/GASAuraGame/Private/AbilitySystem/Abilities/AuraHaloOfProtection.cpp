// Super Copyright


#include "AbilitySystem/Abilities/AuraHaloOfProtection.h"
#include "AuraGameplayTags.h"

FString UAuraHaloOfProtection::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Title>守护光环</> <Level>Lv. %d</>\n\
<SubTitle>In His Name</>\n\
		<Default>造成伤害时, 使最终伤害增加</> <Damage>%.2f%%</>\n\
		<Default>受到伤害时, 使最终伤害减少</> <Damage>%.2f%%</>\n\
		"),
		Level,
		
		ScalableIncreaseDamage.GetValueAtLevel(Level) * 100.f,
		ScalableDecreaseDamage.GetValueAtLevel(Level) * 100.f
	);
}

FString UAuraHaloOfProtection::GetDescriptionNextLevel(int32 Level)
{
	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>In His Name</>\n\
		<Default>造成伤害时, 使最终伤害增加</> <Damage>%.2f%%</>\n\
		<Default>受到伤害时, 使最终伤害减少</> <Damage>%.2f%%</>\n\
		"),
		Level,

		ScalableIncreaseDamage.GetValueAtLevel(Level) * 100.f,
		ScalableDecreaseDamage.GetValueAtLevel(Level) * 100.f
	);
}

void UAuraHaloOfProtection::ActivatePassiveAbility(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FDamageModifier& DamageModifier, const FGameplayTag& TriggerTag)
{
	if (SourceASC)
	{
		FGameplayTagContainer SourceTags;
		SourceASC->GetOwnedGameplayTags(SourceTags);
		if (SourceTags.HasTagExact(FAuraGameplayTags::Get().Abilities_Activating_HaloOfProtection))
		{
			if (ScalableIncreaseDamage.IsValid() && TriggerTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Trigger_Passive_OnCauseDamage))	// 攻击触发时增加伤害
			{
				DamageModifier.DamageIncrease += ScalableIncreaseDamage.GetValueAtLevel(GetAbilityLevel());
				// UE_LOG(LogTemp, Warning, TEXT("%s 造成伤害并触发HaloOfProtection, 造成的伤害增加了 %.2f%% !"), *SourceASC->GetAvatarActor()->GetFName().ToString(), ScalableIncreaseDamage.GetValueAtLevel(GetAbilityLevel()) * 100);
			}
			if (ScalableDecreaseDamage.IsValid() && TriggerTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Trigger_Passive_OnTakeDamage))	// 受伤触发时减少伤害
			{
				DamageModifier.DamageDecrease += ScalableDecreaseDamage.GetValueAtLevel(GetAbilityLevel());
				// UE_LOG(LogTemp, Warning, TEXT("%s 受到伤害并触发HaloOfProtection, 受到的伤害减少了 %.2f%% !"), *TargetASC->GetAvatarActor()->GetFName().ToString(), ScalableDecreaseDamage.GetValueAtLevel(GetAbilityLevel()) * 100);
			}
		}
	}
}
