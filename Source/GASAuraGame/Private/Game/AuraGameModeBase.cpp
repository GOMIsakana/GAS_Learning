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
		UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
	}
}
