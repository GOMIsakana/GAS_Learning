// Super Copyright


#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UAuraGameplayAbilityBase::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("%s <Level>Lv. %d</>"), L"描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n描述占位符\n", 23333);
}

FString UAuraGameplayAbilityBase::GetDescriptionNextLevel(int32 Level)
{
    return FString::Printf(TEXT("<Title>下一级预览</> <Level>Lv. %d</>"), Level);
}

FString UAuraGameplayAbilityBase::GetDescriptionLocked(int32 Level)
{
    return FString::Printf(TEXT("<Title>解锁等级:</> <Level>%d</>"), Level);
}

float UAuraGameplayAbilityBase::GetCooldown(float InLevel)
{
	float CooldownTime = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownTime);
	}
    return CooldownTime;
}

float UAuraGameplayAbilityBase::GetSpellCost(FGameplayAttribute CostAttribute, float InLevel) const
{
	// 施法资源消耗

	float Cost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo ModifierInfo : CostEffect->Modifiers)
		{
			if (ModifierInfo.Attribute == CostAttribute)
			{
				float OutMagnitude = 0.f;
				ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, OutMagnitude);
				Cost -= OutMagnitude;
			}
		}
	}
    return Cost;
}

float UAuraGameplayAbilityBase::GetAdditionalAttributeValueByName(FName AttributeName, float InLevel)
{
	if (SpellAdditionalAttributeMap.Contains(AttributeName))
	{
		return SpellAdditionalAttributeMap[AttributeName].GetValueAtLevel(InLevel);
	}
	return 0.0f;
}
