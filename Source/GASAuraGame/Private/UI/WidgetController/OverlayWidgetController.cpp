// Super Copyright


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnXPChangeDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangeDelegate.AddLambda(
		[this](int32 NewLevel, bool bBroadCastNotify)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bBroadCastNotify);
		}
	);

	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (AAuraGameModeBase* Gamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		Gamemode->SendMapTitleMessageDelegate.AddLambda(
			[this](FString Title, FString SubTitle) 
			{
				OnReceivedTitleMessageDelegate.Broadcast(Title, SubTitle);
			}
		);
	}

	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraAbilitySystemComponent()->AbilityGivenDelegate.AddDynamic(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
					if (Tag.MatchesTag(MessageTag) && !MessageTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message.Multi"))))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						if (Row != nullptr)
						{
							MessageWidgetRowDelegate.Broadcast(*Row);
						}
					}
				}
			}
		);
		GetAuraAbilitySystemComponent()->OnReceiveMultiMessageTagDelegate.AddLambda(
			[this](FGameplayTag MessageTag)
			{
				if (MessageTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message.Multi"))))
				{
					const FMultiUIWidgetRow* Row = GetDataTableRowByTag<FMultiUIWidgetRow>(MultiMessageWidgetDataTable, MessageTag);
					if (Row != nullptr)
					{
						MultiMessageWidgetRowDelegate.Broadcast(*Row);
					}
				}

			}
		);
	}
}

void UOverlayWidgetController::OnXPChanged(float NewXp, bool bBroadcastNotify)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
	if (LevelUpInfo)
	{
		FLevelUpData CurrentLevelData = LevelUpInfo->FindLevelDataForExp(NewXp);
		FLevelUpData NextLevelData = LevelUpInfo->FindLevelDataForExp(NewXp, true);
		bool bIsMaxLevel = CurrentLevelData.TargetLevel == NextLevelData.TargetLevel;
		float XPBarPercentage = 1.f;
		if (!bIsMaxLevel)
		{
			XPBarPercentage = (NewXp - CurrentLevelData.ExperienceRequirement) / (NextLevelData.ExperienceRequirement - CurrentLevelData.ExperienceRequirement);
			XPBarPercentage = FMath::Fmod(XPBarPercentage, 1.f);
		}
		OnXPPercentChangedDelegate.Broadcast(XPBarPercentage);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}