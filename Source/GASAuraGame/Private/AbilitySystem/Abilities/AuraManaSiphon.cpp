// Super Copyright


#include "AbilitySystem/Abilities/AuraManaSiphon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraManaSiphon::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Title>法力虹吸</> <Level>Lv. %d</>\n\
<SubTitle>我吸!我吸!我狠狠吸!</>\n\
		<Default>造成伤害时, 恢复等同于伤害量</> <ManaCost>%.2f%%</> <Default>的法力值</>\n\
		"),
		Level,

		ManaSiphonEffectiveValue.GetValueAtLevel(Level) * 100.f
	);
}

FString UAuraManaSiphon::GetDescriptionNextLevel(int32 Level)
{
	return FString::Printf(TEXT("<Title>升级预览</> <Level>Lv. %d</>\n\
<SubTitle>我吸!我吸!我狠狠吸!</>\n\
		<Default>造成伤害时, 恢复等同于伤害量</> <ManaCost>%.2f%%</> <Default>的法力值</>\n\
		"),
		Level,

		ManaSiphonEffectiveValue.GetValueAtLevel(Level) * 100.f
	);
}

void UAuraManaSiphon::ActivatePassiveAbility(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FDamageModifier& DamageModifier, const FGameplayTag& TriggerTag)
{
	if (SourceASC && SourceASC != TargetASC && ManaSiphonEffectiveValue.IsValid() && ManaSiphonEffect)
	{
		FGameplayTagContainer SourceTags;
		SourceASC->GetOwnedGameplayTags(SourceTags);
		if (SourceTags.HasTagExact(FAuraGameplayTags::Get().Abilities_Activating_ManaSiphon))
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(SourceASC);
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(ManaSiphonEffect, 1.f, ContextHandle);
			// 每级的吸蓝比例在本技能内单独设置
			float EffectiveManaRecoveryValue = DamageModifier.Damage * ManaSiphonEffectiveValue.GetValueAtLevel(GetAbilityLevel());
			// 设置回复量到EffectSpecHandle里面
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FAuraGameplayTags::Get().Attributes_Vital_Mana, EffectiveManaRecoveryValue);
			// 应用回蓝效果
			SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

			// 回血量大于2.5%生命值时才会跳字
			if (EffectiveManaRecoveryValue > SourceASC->GetNumericAttribute(UAuraAttributeSet::GetMaxManaAttribute()) * 0.025)
			{
				// 跳字
				if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(SourceASC->AbilityActorInfo->PlayerController.Get()))
				{
					AuraPC->ClientShowFloatingTextNumber(EffectiveManaRecoveryValue, SourceASC->GetAvatarActor(), FString("+"), FString(""), FColor::Cyan);
				}
			}

			// UE_LOG(LogTemp, Warning, TEXT("%s 造成伤害并触发ManaSiphon, 吸取了 %.2f 法力!"), *SourceASC->GetAvatarActor()->GetFName().ToString(), EffectiveManaRecoveryValue);

		}
	}
}
