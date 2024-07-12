// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/AbilityInfo.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"
#include "LoadScreenSaveGame.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;

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

	UFUNCTION(BlueprintCallable)
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenSaveGame> LoadScreenSaveClass;
};
