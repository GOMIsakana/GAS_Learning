// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraPassiveAbility : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnReceiveDeactivate(const FGameplayTag& AbilityTag);
};
