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

struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, DefensePenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatic().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatic().DefensePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatic().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CritDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatarActor = SourceASC == nullptr ? nullptr : SourceASC->GetAvatarActor();
	AActor* TargetAvatarActor = TargetASC == nullptr ? nullptr : TargetASC->GetAvatarActor();
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatarActor);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatarActor);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float Damage = 0;	// 获取Caller设置的Damage

	for (FGameplayTag DamageTypeTag : FAuraGameplayTags::Get().DamageTypes)
	{
		const float DamageValue = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag);	// 获取Caller设置的Damage
		Damage += DamageValue;
	}

	// 等级差曲线, 等级差越高, 高等级的穿透和防御效率越低
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
	FRealCurve* DefensePenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("DefensePenetration"), FString());
	FRealCurve* DefenseCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveDefense"), FString());
	const float DefensePenetrationCoefficients = DefensePenetrationCurve->Eval(FMath::Max<float>(1.f, SourceCombatInterface->GetCombatLevel() - TargetCombatInterface->GetCombatLevel()));
	const float DefenseCoefficients = DefenseCurve->Eval(FMath::Max<float>(1.f, TargetCombatInterface->GetCombatLevel() - SourceCombatInterface->GetCombatLevel()));

	float TargetDefense = 0.f, SourceDefensePenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefenseDef, EvalParams, TargetDefense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefensePenetrationDef, EvalParams, SourceDefensePenetration);
	Damage += SourceDefensePenetration * DefensePenetrationCoefficients - TargetDefense * DefenseCoefficients;	// 加减法防御

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
