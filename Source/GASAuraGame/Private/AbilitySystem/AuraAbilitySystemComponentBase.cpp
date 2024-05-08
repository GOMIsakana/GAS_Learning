// Super Copyright


#include "AbilitySystem/AuraAbilitySystemComponentBase.h"

void UAuraAbilitySystemComponentBase::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponentBase::OnEffectApplied);
}

void UAuraAbilitySystemComponentBase::OnEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	for (const FGameplayTag Tag : TagContainer)
	{
		FString DebugMsg = FString::Printf(TEXT("Get Tag: %s"), *Tag.GetTagName().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, DebugMsg);
	}
}
