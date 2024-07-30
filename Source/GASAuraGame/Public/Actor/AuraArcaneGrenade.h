// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraArcaneGrenade.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraArcaneGrenade : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Blast", Replicated = true)
	int32 RemainBounceTimes = 5;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
