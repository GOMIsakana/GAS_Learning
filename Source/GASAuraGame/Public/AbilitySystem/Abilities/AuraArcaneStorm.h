// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraArcaneStorm.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraArcaneStorm : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Storm")
	float OverlapIncreaseDamageMagnitude = .05f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Storm")
	float CrystalLifeSpan = 5.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Storm")
	bool bOverrideCrystalLifeSpanByLevel = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Storm")
	float AdditionalCrystalLifeSpanPerLevel = 1.f;

	virtual void SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr) override;

protected:

	virtual void SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile) override;
};
