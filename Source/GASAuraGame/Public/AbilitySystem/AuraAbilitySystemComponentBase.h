// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraAbilitySystemComponentBase : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();

protected:
	void OnEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

};
