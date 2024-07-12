// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, NewWidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndexDelegate;

	UFUNCTION(BlueprintCallable)
	void InitializeLoadSlot();

	UPROPERTY()
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	FString LoadSlotName = FString();

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = 0;
};
