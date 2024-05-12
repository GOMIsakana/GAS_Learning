// Super Copyright


#include "AbilitySystem/MMC/MaxManaMMC.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMaxManaMMC::UMaxManaMMC()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMaxManaMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters AggregatorParams;
	AggregatorParams.SourceTags = SourceTags;
	AggregatorParams.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, AggregatorParams, Intelligence);

	int32 Level = 0;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());
	if (CombatInterface)
	{
		Level = CombatInterface->GetCombatLevel();
	}

	return FMath::Max(0.f, 100.f + Intelligence * 10.f + Level);
}
