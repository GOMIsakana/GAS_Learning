// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASAURAGAME_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(float InXP);
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(float InXP) const;
	UFUNCTION(BlueprintNativeEvent)
	float GetXP();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToCombatLevel(int32 InCombatLevel);
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePointsReward(int32 InAttributePoints);
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPointsReward(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* InMagicCircleMaterial);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& PlayerStartTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddEnemyTargetedAmount(int32 AmountToAdd);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetEnemyTargetedAmount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SendEffectMessage(FGameplayTagContainer TagContainerToSend);
};
