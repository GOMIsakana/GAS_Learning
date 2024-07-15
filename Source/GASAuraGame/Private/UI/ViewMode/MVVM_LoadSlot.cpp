// Super Copyright


#include "UI/ViewMode/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeLoadSlot()
{
	int32 SwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndexDelegate.Broadcast(SwitcherIndex);
}

void UMVVM_LoadSlot::SetPlayerName(const FString& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(const FString& InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMVVM_LoadSlot::SetPlayerStartTag(const FName& InPlayerStartTag)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerStartTag, InPlayerStartTag);
}

void UMVVM_LoadSlot::SetPlayerLevel(const int32& InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InLevel);
}
