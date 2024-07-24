// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetDescriptionNextLevel(int32 Level);
	static FString GetDescriptionLocked(int32 Level);

	UFUNCTION(BlueprintCallable)
	float GetCooldown(float InLevel = 1.f);
	UFUNCTION(BlueprintCallable)
	float GetSpellCost(FGameplayAttribute CostAttribute, float InLevel = 1.f) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAdditionalAttributeValueByName(FName AttributeName, float InLevel = 1.f);
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FScalableFloat> SpellAdditionalAttributeMap;
};
