// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 TargetLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExperienceRequirement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RewardAttributePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RewardSpellPoint;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASAURAGAME_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 包含自动计算等级的功能, 可填入等级并非连续的数组。该函数仅在数组有序时可用
	UFUNCTION(BlueprintCallable)
	FLevelUpData FindLevelDataForExp(float Exp, bool bReturnNextLevel = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelUpData> LevelUpDatas;
};
