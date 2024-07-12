// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	FString LoadSlotName = FString();

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName = FString("Default Name");
};
