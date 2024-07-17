// Super Copyright


#include "Game/LoadScreenSaveGame.h"

bool ULoadScreenSaveGame::GetSavedMapWithMapName(FString InMapName, FSavedMap& OutSavedMap)
{
    for (FSavedMap Map : SavedMaps)
    {
        if (Map.MapAssetName == InMapName)
        {
            OutSavedMap = Map;
            return true;
        }
    }
    return false;
}
