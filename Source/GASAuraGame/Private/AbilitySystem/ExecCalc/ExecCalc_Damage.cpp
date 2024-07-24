// Super Copyright


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"
#include "Character/AuraCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resist_Fire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resist_Lighting);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resist_Arcane);


	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, DefensePenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Resist_Fire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Resist_Lighting, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Resist_Arcane, Target, false);

	}
};

static const AuraDamageStatic& DamageStatic()
{
	// 构建时直接创建一个对应输入的DEF
	static AuraDamageStatic DStatic;

	return DStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatic().PhysicalDamageDef);

	RelevantAttributesToCapture.Add(DamageStatic().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatic().DefensePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatic().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CritDamageDef);

	RelevantAttributesToCapture.Add(DamageStatic().Resist_FireDef);
	RelevantAttributesToCapture.Add(DamageStatic().Resist_LightingDef);
	RelevantAttributesToCapture.Add(DamageStatic().Resist_ArcaneDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_PhysicalDamage, DamageStatic().PhysicalDamageDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_Defense, DamageStatic().DefenseDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_DefensePenetration, DamageStatic().DefensePenetrationDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CritChance, DamageStatic().CritChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CritDamage, DamageStatic().CritDamageDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resist_Fire, DamageStatic().Resist_FireDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resist_Lighting, DamageStatic().Resist_LightingDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resist_Arcane, DamageStatic().Resist_ArcaneDef);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceASC == nullptr ? nullptr : SourceASC->GetAvatarActor();
	AActor* TargetAvatarActor = TargetASC == nullptr ? nullptr : TargetASC->GetAvatarActor();
	int32 SourceCombatLevel = 0;
	int32 TargetCombatLevel = 0;
	if (SourceAvatarActor && SourceAvatarActor->Implements<UCombatInterface>())
	{
		SourceCombatLevel = ICombatInterface::Execute_GetCombatLevel(SourceAvatarActor);
	}
	if (TargetAvatarActor && TargetAvatarActor->Implements<UCombatInterface>())
	{
		TargetCombatLevel = ICombatInterface::Execute_GetCombatLevel(TargetAvatarActor);
	}

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float Damage = 0;	// 获取Caller设置的Damage
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().PhysicalDamageDef, EvalParams, Damage);

	// Debuff相关
	HandleDebuff(ExecutionParams, EffectSpec, EvalParams, TagsToCaptureDefs);

	// 抗性计算
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypeToResist)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag DamageResistTag = Pair.Value;
		if (TagsToCaptureDefs.Contains(DamageResistTag))
		{
			float ResistValue = 0.f;
			const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[DamageResistTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvalParams, ResistValue);
			ResistValue = ResistValue > 100.f ? 100.f : ResistValue;
			float BaseDamageValue = EffectSpec.GetSetByCallerMagnitude(Pair.Key, false);	// 获取Caller设置的Damage
			float ResistDamageValue = BaseDamageValue * (100.f - ResistValue) / 100.f;

			if (ResistDamageValue <= 0.f) continue;

			Damage += ResistDamageValue;
		}
	}
	// 等级差曲线, 等级差越高, 高等级的穿透和防御效率越低
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
	FRealCurve* DefensePenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("DefensePenetration"), FString());
	FRealCurve* DefenseCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveDefense"), FString());
	const float DefensePenetrationCoefficients = DefensePenetrationCurve->Eval(FMath::Max<float>(1.f, SourceCombatLevel - TargetCombatLevel));
	const float DefenseCoefficients = DefenseCurve->Eval(FMath::Max<float>(1.f, TargetCombatLevel - SourceCombatLevel));

	float TargetDefense = 0.f, SourceDefensePenetration = 0.f;
	// 不知道两种方法的区别, 目前测试出来没区别(只是在Modifier中不能像Execution里面获取ASC)
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefenseDef, EvalParams, TargetDefense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefensePenetrationDef, EvalParams, SourceDefensePenetration);
	// TargetDefense = TargetASC->GetNumericAttribute(UAuraAttributeSet::GetDefenseAttribute());
	// SourceDefensePenetration = SourceASC->GetNumericAttribute(UAuraAttributeSet::GetDefensePenetrationAttribute());
	Damage += SourceDefensePenetration * DefensePenetrationCoefficients - TargetDefense * DefenseCoefficients;	// 加减法防御。魔抗没有必要所以删了

	// 获取暴击率, 决定其是否生效
	// 如果它生效, 则增加伤害
	float CritChanceValue = 0.f, CritDamageValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CritChanceDef, EvalParams, CritChanceValue);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CritDamageDef, EvalParams, CritDamageValue);
	CritDamageValue = FMath::Max<float>(0.f, CritDamageValue);
	bool bIsCriticalHit = CritChanceValue > FMath::RandRange(0.f, 100.f);
	if (bIsCriticalHit)
	{
		Damage *= CritDamageValue / 100.f;
	}
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);

	FDamageModifier DamageModifier{};
	DamageModifier.Damage = Damage;
	// 攻击者触发造成伤害时的被动, 并处理伤害
	if (SourceASC)
	{
		if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(SourceASC))
		{
			AuraASC->ActivatePassiveAbilities(TargetASC, DamageModifier, GameplayTags.Abilities_Trigger_Passive_OnCauseDamage);
		}
	}
	// 受击者触发受到伤害时的被动, 并处理伤害
	if (TargetASC)
	{
		if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(TargetASC))
		{
			AuraASC->ActivatePassiveAbilities(SourceASC, DamageModifier, GameplayTags.Abilities_Trigger_Passive_OnTakeDamage);
		}
	}
	Damage = DamageModifier.Damage * (1 + DamageModifier.DamageIncrease - DamageModifier.DamageDecrease);

	if (Damage > 0.f)
	{
		UAuraAbilitySystemLibrary::SetIsDamageValid(EffectContextHandle, true);
	}
	else
	{
		UAuraAbilitySystemLibrary::SetIsDamageValid(EffectContextHandle, false);
		Damage = 0.f;
	}

	const FGameplayModifierEvaluatedData EvalData(UAuraAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, Damage);	// 将Damage增加到该角色的IncomingDamage
	OutExecutionOutput.AddOutputModifier(EvalData);	// 应用EvalData
}

void UExecCalc_Damage::HandleDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, FAggregatorEvaluateParameters EvalParams, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypeToDebuff)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float DamageAmount = EffectSpec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (DamageAmount > -.5f) // 伤害值大于-0.5时才生效
		{
			// 根据几率决定debuff是否生效
			const float DebuffChance = EffectSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Data_Chance, false, -1.f);
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypeToResist[DamageType];
			bool bApplyDebuff = DebuffChance > FMath::RandRange(0.f, 100.f);
			float DamageResistance = -1.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceTag], EvalParams, DamageResistance);

			FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();
			if (bApplyDebuff)
			{

				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Data_Damage, false, -1.f);
				const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Data_Duration, false, -1.f);
				const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(GameplayTags.Debuff_Data_Frequency, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
			else
			{
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, false);
			}
		}
	}
}