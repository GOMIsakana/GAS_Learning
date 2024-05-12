// Super Copyright


#include "AbilitySystem/MMC/MaxHealthMMC.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMaxHealthMMC::UMaxHealthMMC()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMaxHealthMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters AggregatorParams;
	AggregatorParams.SourceTags = SourceTags;
	AggregatorParams.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, AggregatorParams, Vigor);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 PlayerLevel = 0;
	if (CombatInterface)
	{
		PlayerLevel = CombatInterface->GetCombatLevel();
	}

	// 100 + 等级 + 活力 * 10， 最低为 1
	return FMath::Max(1.f, 100.f + PlayerLevel + Vigor * 10.f);
}
