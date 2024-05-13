// Super Copyright


#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponentBase::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponentBase::OnEffectApplied);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_PhysicalDamage.ToString()));
}

void UAuraAbilitySystemComponentBase::OnEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
