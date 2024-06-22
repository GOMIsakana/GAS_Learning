// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellPointSignature, int32, SpellPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bEnableLevelupButton, bool, bEnableEquipButton);

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

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
	UFUNCTION()
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoint, bool& bShouldEnableLevelupButton, bool& bShouldEnableEquipButton);
};
