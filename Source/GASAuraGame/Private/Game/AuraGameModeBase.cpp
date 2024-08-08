// Super Copyright


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Game/AuraGameInstance.h"
#include "EngineUtils.h"
#include "Interaction/SaveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "GameFramework/Character.h"
#include "UI/WidgetController/OverlayWidgetController.h"

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
		LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
		LoadScreenSaveGame->SlotStatus = LoadSlot->SlotStatus;
		LoadScreenSaveGame->PlayerStartTag = LoadSlot->GetPlayerStartTag();
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
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, GameMaps[LoadSlot->GetMapName()].MapAsset);
	// 显示新地图的名称
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData() const
{
	if (UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		return GetSaveSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
	}
	return nullptr;
}

void AAuraGameModeBase::SaveInGameSaveData(ULoadScreenSaveGame* SaveObject)
{
	if (UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		FString LoadSlotName = GameInstance->LoadSlotName;
		int32 LoadSlotIndex = GameInstance->LoadSlotIndex;
		GameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

		UGameplayStatics::SaveGameToSlot(SaveObject, LoadSlotName, LoadSlotIndex);
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* WorldToSave, const FString& DestinationMapAssetName)
{
	if (WorldToSave == nullptr) return;
	FString WorldName = WorldToSave->GetMapName();
	WorldName.RemoveFromStart(WorldToSave->StreamingLevelsPrefix);
	UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	if (ULoadScreenSaveGame* GameSave = GetSaveSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex))
	{
		// 如果是进入其他地图的存档(第二个输入不为空), 则将游戏存档中的目标地图名称改为新地图
		if (DestinationMapAssetName != FString(""))
		{
			GameSave->MapAssetName = DestinationMapAssetName;
			GameSave->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		FSavedMap SavedMap;
		// 没找到就原地新建一个
		if (!GameSave->GetSavedMapWithMapName(WorldName, SavedMap))
		{
			SavedMap.MapAssetName = WorldName;
			GameSave->SavedMaps.Add(SavedMap);
		}
		SavedMap.MapSavedActors.Empty();
		
		for (FActorIterator It(WorldToSave); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;	// 如果是不需要存档的Actor, 则直接跳过

			// 创建新的ActorSave, 并保存名称和Transform
			FSavedActor ActorToSave;
			ActorToSave.ActorName = Actor->GetFName();
			ActorToSave.ActorTransform = Actor->GetActorTransform();

			// 创建Archive并Serialize数据, 然后保存到ActorSave
			// 
			// 创建MemoryWriter/MemoryReader -> 将MemoryArchive转化为Archive -> 设置Arcive.ArIsSaveGame为true -> 
			// 让需要序列化的对象执行Serialize(Archive), 就会对其变量进行写/读操作(写是写入到创建MemoryWriter时传入的TArray<uint8>中, 读是读取变量还原到执行Serialize的对象中)
			// 
			// 将需要保存的Actor的Serialize作为引用传入其中, 使用MemoryWriter去进行序列化
			FMemoryWriter MemoryWriter(ActorToSave.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);	// 新建Archive, 该结构的Archive会将Object和FName转化为FString进行储存
			Archive.ArIsSaveGame = true; // 将该Archive其设置为保存游戏状态

			Actor->Serialize(Archive);	// 将Actor的序列化信息存入Archive中（需要用MemoryReader创建的Archive才会进行反序列化）

			SavedMap.MapSavedActors.AddUnique(ActorToSave);	// 将处理好的FSavedActor进行储存
		}
		// 替换掉同名的地图数据为新的地图数据
		for (FSavedMap& MapToReplace : GameSave->SavedMaps)
		{
			if (MapToReplace.MapAssetName == SavedMap.MapAssetName)
			{
				MapToReplace = SavedMap;
			}
		}
		// 替换完毕, 进行存档
		UGameplayStatics::SaveGameToSlot(GameSave, GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* WorldToLoad)
{
	if (WorldToLoad == nullptr) return;
	FString WorldName = WorldToLoad->GetMapName();
	WorldName.RemoveFromStart(WorldToLoad->StreamingLevelsPrefix);
	UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	if (UGameplayStatics::DoesSaveGameExist(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex))
	{
		if (ULoadScreenSaveGame* GameSave = GetSaveSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex))
		{
			FSavedMap SavedMap;
			if (GameSave->GetSavedMapWithMapName(WorldName, SavedMap))
			{
				// 双重循环, 外层检测世界中的所有Actor, 内层检测所有保存的Actor
				for (FActorIterator It(WorldToLoad); It; ++It)
				{
					AActor* Actor = *It;

					if (!Actor->Implements<USaveInterface>()) continue;	// 如果是不需要存档的Actor, 则直接跳过

					for (FSavedActor SavedActor : SavedMap.MapSavedActors)
					{
						if (Actor->GetFName() == SavedActor.ActorName)
						{
							// 由实现了这个接口的actor决定是否初始化自身位置
							if (ISaveInterface::Execute_bShouldLoadTransform(Actor))
							{
								Actor->SetActorTransform(SavedActor.ActorTransform);
							}
							// 此处的Reader是用于读取存档的Archive, 与Writer相对
							FMemoryReader MemoryReader(SavedActor.Bytes);

							FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
							Archive.ArIsSaveGame = true;

							Actor->Serialize(Archive);	// 逆操作, 将之前保存在序列化数据集中的数据还原为变量（需要用MemoryReader创建的Archive才会进行还原操作）

							ISaveInterface::Execute_LoadActor(Actor);	// 通知被加载的Actor进行初始化
						}
					}
				}
			}
		}
	}
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(FString MapAssetName)
{
	for (TTuple<FString, FMapInfo> Map : GameMaps)
	{
		if (Map.Value.MapAsset.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance());
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
				if (GameInstance && PlayerStart->PlayerStartTag == GameInstance->PlayerStartTag)
				{
					ChosenPlayerStart = PlayerStart;
					break;
				}
			}
		}
	}
	return ChosenPlayerStart;
}

void AAuraGameModeBase::PlayerDied(ACharacter* DeadPlayer)
{
	ULoadScreenSaveGame* GameSave = RetrieveInGameSaveData();
	if (!IsValid(GameSave)) return;

	UGameplayStatics::OpenLevel(DeadPlayer, FName(GameSave->MapAssetName));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FMapInfo DefaultMapInfo;
	DefaultMapInfo.MapAsset = DefaultMap;
	DefaultMapInfo.MapTitleOverride = StartupMapTitleOverride;
	DefaultMapInfo.MapSubTitle = StartupMapSubTitle;
	GameMaps.Add(StartupMapName, DefaultMapInfo);

	if (bBroadcastSendTitleMessage)
	{
		SendMapTitleMessage();
	}
}

void AAuraGameModeBase::SendMapTitleMessage()
{
	if (UAuraGameInstance* GameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		if (ULoadScreenSaveGame* GameSave = GetSaveSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex))
		{
			FString MapName = GetMapNameFromMapAssetName(GameSave->MapAssetName);
			if (GameMaps.Contains(MapName))
			{
				FMapInfo MapInfo = GameMaps[MapName];
				FString Title = MapInfo.MapTitleOverride == FString("") ? MapName : MapInfo.MapTitleOverride;
				SendMapTitleMessageDelegate.Broadcast(Title, MapInfo.MapSubTitle);
				bBroadcastSendTitleMessage = false;
			}
			else
			{
				bBroadcastSendTitleMessage = true;
			}
		}
	}
}
