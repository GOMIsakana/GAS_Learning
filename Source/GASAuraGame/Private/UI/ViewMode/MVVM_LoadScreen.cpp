// Super Copyright


#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	if (LoadSlotClass == nullptr) return;

	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");

	LoadSlotMap.Add(0, LoadSlot_0);
	LoadSlotMap.Add(1, LoadSlot_1);
	LoadSlotMap.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotByIndex(int32 Index)
{
	return LoadSlotMap.FindChecked(Index);
}

void UMVVM_LoadScreen::CreateNewSaveButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* Gamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlotMap[SlotIndex]->PlayerName = EnteredName;

	Gamemode->SaveSlotData(LoadSlotMap[SlotIndex], SlotIndex);
	LoadSlotMap[SlotIndex]->SetWidgetSwitcherIndexDelegate.Broadcast(2);
}

void UMVVM_LoadScreen::ToCreateButtonPressed(int32 SlotIndex)
{
	LoadSlotMap[SlotIndex]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSaveButtonPressed(int32 SlotIndex)
{

}
