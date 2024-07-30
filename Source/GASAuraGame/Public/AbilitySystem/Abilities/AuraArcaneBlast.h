// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraArcaneBlast.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraArcaneBlast : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

	virtual void SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr) override;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Blast")
	int32 MaxBounceTimes = 5;

protected:
	virtual void SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile) override;
};
