// Super Copyright


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AuraGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetAuraPlayerState()->OnSpellPointChangeDelegate.AddLambda(
		[this](int32 NewSpellPoint)
		{
			SpellPointDelegate.Broadcast(NewSpellPoint);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoint = GetAuraPlayerState()->GetSpellPoint();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetAbilitySpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
	}
	bool bEnableLevelupButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, GetAuraPlayerState()->GetSpellPoint(), bEnableLevelupButton, bEnableEquipButton);
	SpellGlobeSelectedDelegate.Broadcast(bEnableLevelupButton, bEnableEquipButton);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoint, bool& bShouldEnableLevelupButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableLevelupButton = false;
		bShouldEnableEquipButton = false;
		return;
	}
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		if (SpellPoint > 0)
		{
			bShouldEnableLevelupButton = true;
		}
		bShouldEnableEquipButton = true;
		return;
	}
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableLevelupButton = true;
		bShouldEnableEquipButton = false;
		return;
	}
}
