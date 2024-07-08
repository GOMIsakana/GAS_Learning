// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	int32 AdditionalShardsAmount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float AdditionalShardsSpawnRadius = 400.f;
};
