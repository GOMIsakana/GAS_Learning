// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MaxManaMMC.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UMaxManaMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMaxManaMMC();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
