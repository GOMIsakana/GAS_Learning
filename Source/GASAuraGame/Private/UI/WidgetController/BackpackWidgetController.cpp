// Super Copyright


#include "UI/WidgetController/BackpackWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Interaction/BackpackInterface.h"

void UBackpackWidgetController::SetItemToTargetSlot(FBackpackItem InItem, int32 TargetSlot)
{
	if (AAuraPlayerController* AuraPC = GetAuraPlayerController())
	{
		if (AuraPC->Implements<UBackpackInterface>())
		{
			IBackpackInterface::Execute_SetItemAtBackpackSlot(AuraPC, InItem, TargetSlot);
		}
	}
}

void UBackpackWidgetController::BindCallbacksToDependencies()
{
	if (AAuraPlayerController* AuraPC = GetAuraPlayerController())
	{
		if (IBackpackInterface* BackpackInterface = Cast<IBackpackInterface>(AuraPC))
		{
			BackpackInterface->GetBackpackItemMovedDelegate().AddLambda(
				[this](int32 SourceItemSlot, int32 TargetItemSlot)
				{
					OnBackpackItemMovedDelegate.Broadcast(SourceItemSlot, TargetItemSlot);
				}
			);
		}
	}
}
