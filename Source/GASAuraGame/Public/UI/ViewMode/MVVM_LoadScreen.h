// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectButtonPressedSignature);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void InitializeLoadSlots();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotClass;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void CreateNewSaveButtonPressed(int32 SlotIndex, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void ToCreateButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSaveButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void LoadSlot();

	UPROPERTY(BlueprintAssignable)
	FSelectButtonPressedSignature SelectButtonPressedDelegate;

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlotMap;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
};
