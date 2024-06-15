// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Data/LevelUpInfo.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatPlayerStateChangeSignature, float/* NewFloat */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnIntPlayerStateChangeSignature, int32/* NewInt32 */);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetCombatLevel() const { return Level; }

	FORCEINLINE float GetXP() const { return XP; }
	void SetXP(float NewXP);
	void SetCombatLevel(int32 NewLevel);

	FOnFloatPlayerStateChangeSignature OnXPChangeDelegate;
	FOnIntPlayerStateChangeSignature OnLevelChangeDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Data")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	float XP;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(float OldXP);
};
