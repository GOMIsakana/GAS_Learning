// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "HealthRegenerationMMC.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UHealthRegenerationMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UHealthRegenerationMMC();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
