// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "AuraHaloOfProtection.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraHaloOfProtection : public UAuraPassiveAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

	virtual void ActivatePassiveAbility(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FDamageModifier& DamageModifier, const FGameplayTag& TriggerTag) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat ScalableIncreaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat ScalableDecreaseDamage;
};
