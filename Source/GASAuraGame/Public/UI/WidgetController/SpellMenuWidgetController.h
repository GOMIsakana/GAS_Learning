// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellPointSignature, int32, SpellPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FSpellGlobeSelectedSignature, bool, bEnableLevelupButton, bool, bEnableEquipButton, FString, Description, FString, DescriptionNextLevel, bool, bDeselect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignSignature, const FGameplayTag&, AbilityType);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASAURAGAME_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FSpellPointSignature SpellPointDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FWaitForEquipSelectionSignature WaitForEquippedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FWaitForEquipSelectionSignature StopWaitForEquippedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FSpellGlobeReassignSignature SpellGlobeReassignDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();

	UFUNCTION(BlueprintCallable)
	void EquippedButtonPressed();

	UFUNCTION(BlueprintCallable)
	void LevelupButtonClicked();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	UFUNCTION()
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoint, bool& bShouldEnableLevelupButton, bool& bShouldEnableEquipButton);

	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoint = 0;

	FString DefaultDescription = TEXT("<SubTitle>选择一个技能以查看描述</>");

	bool bWaitingForEquippedSelection = false;

	FGameplayTag SelectedSlot;
};
