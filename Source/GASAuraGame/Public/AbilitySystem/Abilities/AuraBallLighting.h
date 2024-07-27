// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraLightingBall.h"
#include "AuraGameplayTags.h"
#include "GameplayTags.h"
#include "AuraBallLighting.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraBallLighting : public UAuraProjectileSpell
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Lighting")
	TSubclassOf<AAuraLightingBall> LightingBallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Lighting")
	FGameplayTag LightingBallGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball Lighting")
	float LightingBallLifeSpan = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball Lighting")
	float MaxConnectingDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball Lighting")
	float TargetSearchingDistance = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball Lighting")
	float DealDamageFrequency = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ball Lighting")
	int32 MaxTargetAmount = 3;

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

protected:
	virtual void SpawnProjectileWithSpread(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget) override;
	virtual void SetupDamageEffectParamsForProjectile(AAuraProjectile* InProjectile) override;
};
