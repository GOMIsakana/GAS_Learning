// Super Copyright


#include "AbilitySystem/Abilities/AuraLifeSiphon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraLifeSiphon::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Title>生命虹吸</> <Level>Lv. %d</>\n\
<SubTitle>我吸!我吸!我狠狠吸!</>\n\
		<Default>造成伤害时, 恢复等同于伤害量</> <HealthCost>%.2f%%</> <Default>的生命值</>\n\
		"),
		Level,

		LifeSiphonEffectiveValue.GetValueAtLevel(Level) * 100.f
	);
}

FString UAuraLifeSiphon::GetDescriptionNextLevel(int32 Level)
{
	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>我吸!我吸!我狠狠吸!</>\n\
		<Default>造成伤害时, 恢复等同于伤害量</> <HealthCost>%.2f%%</> <Default>的生命值</>\n\
		"),
		Level,

		LifeSiphonEffectiveValue.GetValueAtLevel(Level) * 100.f
	);
}

void UAuraLifeSiphon::ActivatePassiveAbility(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FDamageModifier& DamageModifier, const FGameplayTag& TriggerTag)
{
	if (SourceASC && SourceASC != TargetASC && LifeSiphonEffectiveValue.IsValid() && LifeSiphonEffect)	// 避免自己吸自己血
	{
		FGameplayTagContainer SourceTags;
		SourceASC->GetOwnedGameplayTags(SourceTags);
		if (SourceTags.HasTagExact(FAuraGameplayTags::Get().Abilities_Activating_LifeSiphon))
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(SourceASC);
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(LifeSiphonEffect, 1.f, ContextHandle);
			// 每级的吸血比例在本技能内单独设置
			float EffectiveHealthRecoveryValue = DamageModifier.Damage * LifeSiphonEffectiveValue.GetValueAtLevel(GetAbilityLevel());
			// 设置回复量到EffectSpecHandle里面
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FAuraGameplayTags::Get().Attributes_Vital_Health, EffectiveHealthRecoveryValue);
			// 应用回血效果
			SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

			// 回血量大于2.5%最大生命值时才会跳字
			if (EffectiveHealthRecoveryValue > SourceASC->GetNumericAttribute(UAuraAttributeSet::GetMaxHealthAttribute()) * 0.025)
			{
				// 跳字
				if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(SourceASC->AbilityActorInfo->PlayerController.Get()))
				{
					AuraPC->ClientShowFloatingTextNumber(EffectiveHealthRecoveryValue, SourceASC->GetAvatarActor(), FString("+"), FString(""), FColor::Green);
				}
			}

			// UE_LOG(LogTemp, Warning, TEXT("%s 造成伤害并触发LifeSiphon, 吸取了 %.2f 血量!"), *SourceASC->GetAvatarActor()->GetFName().ToString(), EffectiveHealthRecoveryValue);
		}
	}
}
