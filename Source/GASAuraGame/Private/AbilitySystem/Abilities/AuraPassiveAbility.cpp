// Super Copyright


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(GetAbilitySystemComponentFromActorInfo()))
	{
		AuraASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UAuraPassiveAbility::OnReceiveDeactivate);
	}
}

void UAuraPassiveAbility::OnReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
