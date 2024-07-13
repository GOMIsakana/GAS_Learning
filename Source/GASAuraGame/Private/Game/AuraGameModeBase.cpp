// Super Copyright


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 若同一位置存在旧存档，则将其删除
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	// 尝试存档
	if (LoadScreenSaveClass)
	{
		// 创建新的存档对象, 并将其转化为本项目专有的存档对象
		USaveGame* GameSaveObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveClass);
		ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(GameSaveObject);
		// 将存档对象的槽位名称改为对应存档槽位的名称
		LoadScreenSaveGame->LoadSlotName = LoadSlot->LoadSlotName;
		LoadScreenSaveGame->PlayerName = LoadSlot->PlayerName;
		LoadScreenSaveGame->SlotStatus = LoadSlot->SlotStatus;
		UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* GameSaveObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		GameSaveObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else if (LoadScreenSaveClass)
	{
		GameSaveObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(GameSaveObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
}
