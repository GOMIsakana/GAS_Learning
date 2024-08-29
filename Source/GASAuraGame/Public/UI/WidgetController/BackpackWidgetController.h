// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Data/DropItems.h"
#include "BackpackWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBackpackItemMovedSigenature, int32, SourceItemSlot, int32, TargetItemSlot);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASAURAGAME_API UBackpackWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;

	UFUNCTION(BlueprintCallable)
	void SetItemToTargetSlot(FBackpackItem InItem, int32 TargetSlot);

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDropItems> DropItemAsset;

	UPROPERTY(BlueprintAssignable)
	FOnBackpackItemMovedSigenature OnBackpackItemMovedDelegate;
};
