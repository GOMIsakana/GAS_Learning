// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraLightingStorm.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraLightingStorm : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Lighting Storm")
	float AdditionalRadiusPerLevel = 50.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Lighting Storm")
	bool bDamageRadiusOverrideByLevel = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Lighting Storm")
	float OverrideRadius = 1000.f;
};
