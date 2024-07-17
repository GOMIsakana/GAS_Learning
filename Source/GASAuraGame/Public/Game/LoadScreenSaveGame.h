// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Abilities/GameplayAbility.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AbilityLevel;
};

inline bool operator==(const FSavedAbility& A, const FSavedAbility& B)
{
	return A.AbilityTag.MatchesTagExact(B.AbilityTag);
}

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> Bytes;	/* 保存了Actor的Serialized的变量(这些变量需要用宏标记为SaveGame才会被序列化保存) */
};

inline bool operator==(const FSavedActor& A, const FSavedActor& B)
{
	return A.ActorName == B.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	TArray<FSavedActor> MapSavedActors;
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

	/* 技能数据 */

	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	/* 地图数据 */
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	// 找到地图时返回true, 未找到地图时返回false
	bool GetSavedMapWithMapName(FString InMapName, FSavedMap& OutSavedMap);
};
