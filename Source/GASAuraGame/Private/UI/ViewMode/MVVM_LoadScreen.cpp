// Super Copyright


#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameInstance.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	if (LoadSlotClass == nullptr) return;

	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;

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
	if (IsValid(Gamemode))
	{
		LoadSlotMap[SlotIndex]->SetPlayerName(EnteredName);
		LoadSlotMap[SlotIndex]->SlotStatus = ESaveSlotStatus::Taken;
		LoadSlotMap[SlotIndex]->SetMapName(Gamemode->StartupMapName);
		LoadSlotMap[SlotIndex]->SetPlayerStartTag(Gamemode->DefaultMapPlayerStartTag);
		LoadSlotMap[SlotIndex]->SetPlayerLevel(1);
		LoadSlotMap[SlotIndex]->MapAssetName = Gamemode->DefaultMap.ToSoftObjectPath().GetAssetName();

		Gamemode->SaveSlotData(LoadSlotMap[SlotIndex], SlotIndex);
		LoadSlotMap[SlotIndex]->InitializeLoadSlot();

		if (UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(Gamemode->GetGameInstance()))
		{
			GameInstance->PlayerStartTag = Gamemode->DefaultMapPlayerStartTag;
			GameInstance->LoadSlotName = LoadSlotMap[SlotIndex]->LoadSlotName;
			GameInstance->LoadSlotIndex = SlotIndex;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString::Printf(TEXT("请在单人模式下进行游戏")));
	}
}

void UMVVM_LoadScreen::ToCreateButtonPressed(int32 SlotIndex)
{
	LoadSlotMap[SlotIndex]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSaveButtonPressed(int32 SlotIndex)
{
	SelectButtonPressedDelegate.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> Slot : LoadSlotMap)
	{
		Slot.Value->EnableSelectButtonDelegate.Broadcast(Slot.Key != SlotIndex);
	}
	SelectedSlot = LoadSlotMap[SlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		if (AAuraGameModeBase* Gamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			Gamemode->DeleteSlotData(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		}
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeLoadSlot();
		SelectedSlot->EnableSelectButtonDelegate.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGamemode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (SelectedSlot && IsValid(AuraGamemode))
	{
		if (UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(AuraGamemode->GetGameInstance()))
		{
			GameInstance->PlayerStartTag = SelectedSlot->GetPlayerStartTag();
			GameInstance->LoadSlotName = SelectedSlot->LoadSlotName;
			GameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
		}
		if (SelectedSlot)
		{
			AuraGamemode->TravelToMap(SelectedSlot);
		}
	}
}

void UMVVM_LoadScreen::LoadSlot()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> Slot : LoadSlotMap)
	{
		ULoadScreenSaveGame* GameSaveObject = AuraGameMode->GetSaveSlotData(Slot.Value->LoadSlotName, Slot.Key);

		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = GameSaveObject->SlotStatus;
		const FString PlayerName = GameSaveObject->PlayerName;
		const FString MapName = GameSaveObject->MapName;
		const FName PlayerStartTag = GameSaveObject->PlayerStartTag;
		const int32 PlayerLevel = GameSaveObject->CombatLevel;
		const FString MapAssetName = GameSaveObject->MapAssetName;

		Slot.Value->SlotStatus = SaveSlotStatus;
		Slot.Value->SetPlayerLevel(PlayerLevel);
		Slot.Value->SetPlayerName(PlayerName);
		Slot.Value->SetMapName(MapName);
		Slot.Value->SetPlayerStartTag(PlayerStartTag);
		Slot.Value->MapAssetName = MapAssetName;
		Slot.Value->InitializeLoadSlot();
	}
}
