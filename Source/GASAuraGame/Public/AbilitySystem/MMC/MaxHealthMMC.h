// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MaxHealthMMC.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UMaxHealthMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMaxHealthMMC();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
