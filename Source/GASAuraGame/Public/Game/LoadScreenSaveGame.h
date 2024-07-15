// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

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

	UPROPERTY(BlueprintReadOnly)
	FString MapName = FString("Default Dungeon Name");

	UPROPERTY(BlueprintReadOnly)
	FName PlayerStartTag;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ESaveSlotStatus> SlotStatus = Vacant;

	UPROPERTY(BlueprintReadOnly)
	bool bFirstTimeLoadIn = true;

	/* 玩家数据(来自PlayerState) */

	UPROPERTY()
	int32 CombatLevel = 0;

	UPROPERTY()
	float CombatXP = 0.f;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	/* 玩家数据(来自AttribuetSet) */

	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;
};
