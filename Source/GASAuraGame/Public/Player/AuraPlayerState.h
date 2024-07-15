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

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatPlayerStateChangeSignature, float/* NewFloat */, bool/* bBroadcastToNotify */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnIntPlayerStateChangeSignature, int32/* NewInt32 */, bool/* bBroadcastToNotify */);

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
	void SetCombatLevel(int32 NewLevel, bool bBroadcastLevelupNotify = true);

	FORCEINLINE float GetXP() const { return XP; }
	void SetXP(float NewXP);

	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
	void SetAttributePoint(int32 NewAttributePoint);
	FORCEINLINE int32 GetSpellPoint() const { return SpellPoint; }
	void SetSpellPoint(int32 NewSpellPoint);

	FOnIntPlayerStateChangeSignature OnLevelChangeDelegate;
	FOnFloatPlayerStateChangeSignature OnXPChangeDelegate;
	FOnIntPlayerStateChangeSignature OnAttributePointChangeDelegate;
	FOnIntPlayerStateChangeSignature OnSpellPointChangeDelegate;

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
	float XP = 0.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoint)
	int32 SpellPoint = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(float OldXP);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	UFUNCTION()
	void OnRep_SpellPoint(int32 OldSpellPoint);
};
