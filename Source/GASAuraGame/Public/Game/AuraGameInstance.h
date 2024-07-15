// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString LoadSlotName;

	UPROPERTY()
	int32 LoadSlotIndex;
};
