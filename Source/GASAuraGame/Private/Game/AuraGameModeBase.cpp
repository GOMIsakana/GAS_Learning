// Super Copyright


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 若同一位置存在旧存档，则将其删除
	DeleteSlotData(LoadSlot->LoadSlotName, SlotIndex);
	// 尝试存档
	if (LoadScreenSaveClass)
	{
		// 创建新的存档对象, 并将其转化为本项目专有的存档对象
		USaveGame* GameSaveObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveClass);
		ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(GameSaveObject);
		// 将存档对象的槽位名称改为对应存档槽位的名称
		LoadScreenSaveGame->LoadSlotName = LoadSlot->LoadSlotName;
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
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

void AAuraGameModeBase::DeleteSlotData(FString SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, GameMaps[LoadSlot->GetMapName()]);
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Actors);
	AActor* ChosenPlayerStart = nullptr;
	if (Actors.Num() > 0)
	{
		ChosenPlayerStart = Actors[0];
		for (AActor* FoundActor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor))
			{
				if (PlayerStart->PlayerStartTag == FName("Placeholder"))
				{
					ChosenPlayerStart = PlayerStart;
					break;
				}
			}
		}
	}
	return ChosenPlayerStart;
}

void AAuraGameModeBase::BeginPlay()
{
	GameMaps.Add(StartupMapName, DefaultMap);
}
