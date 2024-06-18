// Super Copyright


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "UObject/UnrealType.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Value(), Pair.Key);
	}
	if (AuraPlayerState)
	{
		AttributePointDelegate.Broadcast(AuraPlayerState->GetAttributePoint());
		SpellPointDelegate.Broadcast(AuraPlayerState->GetSpellPoint());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	if (AuraPlayerState)
	{
		AuraPlayerState->OnAttributePointChangeDelegate.AddLambda(
			[this](int32 Points)
			{
				AttributePointDelegate.Broadcast(Points);
			}
		);
		AuraPlayerState->OnSpellPointChangeDelegate.AddLambda(
			[this](int32 Points)
			{
				SpellPointDelegate.Broadcast(Points);
			}
		);
	}

	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Value(), Pair.Key);
			}
		);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (UAuraAbilitySystemComponentBase* AuraASC = Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent))
	{
		AuraASC->UpgradeAttribute(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute, const FGameplayTag Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	// 获取Attribute在某个AS中的值（具体的属性在AS里面已经写好（AS.GetMaxHealthAttribute这种）并加入到map里面了）
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
