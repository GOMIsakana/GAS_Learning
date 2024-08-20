// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/AbilityInfo.h"
#include "Data/LootTiers.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"
#include "LoadScreenSaveGame.h"
#include "MediaSource.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> MapAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapTitleOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapSubTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMediaSource> MapBackgroundMusicOverride;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FSendMapTitleMessageSignature, FString /*Title*/, FString /* Subtitle */);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterClassDefault")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityInfo")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "LootTiers")
	TObjectPtr<ULootTiers> LootTiers;

	FSendMapTitleMessageSignature SendMapTitleMessageDelegate;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	void DeleteSlotData(FString SlotName, int32 SlotIndex);

	void TravelToMap(UMVVM_LoadSlot* LoadSlot);

	ULoadScreenSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameSaveData(ULoadScreenSaveGame* SaveObject);

	void SaveWorldState(UWorld* WorldToSave, const FString& DestinationMapAssetName = FString(""));

	void LoadWorldState(UWorld* WorldToLoad);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenSaveGame> LoadScreenSaveClass;

	UPROPERTY(EditDefaultsOnly)
	FString StartupMapName;

	UPROPERTY(EditDefaultsOnly)
	FString StartupMapTitleOverride;

	UPROPERTY(EditDefaultsOnly)
	FString StartupMapSubTitle;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultMapPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> DefaultMapBackgroundMusicOverride;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, FMapInfo> GameMaps;

	FString GetMapNameFromMapAssetName(FString MapAssetName);

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(ACharacter* DeadPlayer);

	UFUNCTION(BlueprintCallable)
	void SendMapTitleMessage();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "WorldContextObject"))
	UMediaSource* GetMapBackgroundMusic(UObject* WorldContextObject);
protected:
	virtual void BeginPlay() override;

	bool bBroadcastSendTitleMessage = false;
};
