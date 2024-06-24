// Super Copyright


#include "UI/WidgetController/SpellMenuWidgetController.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.StatusTag = StatusTag;
				bool bEnableLevelupButton = false;
				bool bEnableEquipButton = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoint, bEnableLevelupButton, bEnableEquipButton);
				FString Description;
				FString DescriptionNextLevel;
				GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
				SpellGlobeSelectedDelegate.Broadcast(bEnableLevelupButton, bEnableEquipButton, Description, DescriptionNextLevel, false);
			}
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	GetAuraPlayerState()->OnSpellPointChangeDelegate.AddLambda(
		[this](int32 NewSpellPoint)
		{
			SpellPointDelegate.Broadcast(NewSpellPoint);
			CurrentSpellPoint = NewSpellPoint;

			bool bEnableLevelupButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoint, bEnableLevelupButton, bEnableEquipButton);
			FString Description;
			FString DescriptionNextLevel;
			GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
			SpellGlobeSelectedDelegate.Broadcast(bEnableLevelupButton, bEnableEquipButton, Description, DescriptionNextLevel, false);
		}
	);
	GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquippedSelection)
	{
		const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquippedDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquippedSelection = false;
	}

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
	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	bool bEnableLevelupButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, GetAuraPlayerState()->GetSpellPoint(), bEnableLevelupButton, bEnableEquipButton);
	FString Description;
	FString DescriptionNextLevel;
	GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
	SpellGlobeSelectedDelegate.Broadcast(bEnableLevelupButton, bEnableEquipButton, Description, DescriptionNextLevel, false);
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
	if (bWaitingForEquippedSelection)
	{
		const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquippedDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquippedSelection = false;
	}

	SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	SpellGlobeSelectedDelegate.Broadcast(false, false, DefaultDescription, DefaultDescription, true);
}

void USpellMenuWidgetController::EquippedButtonPressed()
{
	const FGameplayTag& AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForEquippedDelegate.Broadcast(AbilityType);
	bWaitingForEquippedSelection = true;

	// const FGameplayTag& SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedAbility.StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::LevelupButtonClicked()
{
	if (GetAuraAbilitySystemComponent() && !SelectedAbility.AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		GetAuraAbilitySystemComponent()->ServerLevelupAbility(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquippedSelection) return;

	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
	if (!AbilityType.MatchesTagExact(SelectedAbilityType)) return;

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
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

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquippedSelection = false;

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitForEquippedDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityType);

	// 清空之前选中的技能
	SpellGlobeDeselected();
	SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
}