// Super Copyright


#include "AbilitySystem/MMC/HealthRegenerationMMC.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UHealthRegenerationMMC::UHealthRegenerationMMC()
{
	StrengthDef.AttributeToCapture = UAuraAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
	RelevantAttributesToCapture.Add(VigorDef);
}

float UHealthRegenerationMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters AggregatorParams;
	AggregatorParams.SourceTags = SourceTag;
	AggregatorParams.TargetTags = TargetTag;

	float Strength = 0.f;
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, AggregatorParams, Strength);
	GetCapturedAttributeMagnitude(VigorDef, Spec, AggregatorParams, Vigor);

	return 1.f + Strength * .05f + Vigor * .1f ;
}
