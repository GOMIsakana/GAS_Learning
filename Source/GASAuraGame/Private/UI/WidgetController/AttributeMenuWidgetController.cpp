// Super Copyright


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "UObject/UnrealType.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Value(), Pair.Key);
	}
	if (GetAuraPlayerState())
	{
		AttributePointDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoint());
		SpellPointDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoint());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (GetAuraPlayerState())
	{
		GetAuraPlayerState()->OnAttributePointChangeDelegate.AddLambda(
			[this](int32 Points, bool bBroadcastNotify)
			{
				AttributePointDelegate.Broadcast(Points);
			}
		);
		GetAuraPlayerState()->OnSpellPointChangeDelegate.AddLambda(
			[this](int32 Points, bool bBroadcastNotify)
			{
				SpellPointDelegate.Broadcast(Points);
			}
		);
	}

	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributes)
	{
		GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Value(), Pair.Key);
			}
		);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute, const FGameplayTag Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	// 获取Attribute在某个AS中的值（具体的属性在AS里面已经写好（AS.GetMaxHealthAttribute这种）并加入到map里面了）
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
