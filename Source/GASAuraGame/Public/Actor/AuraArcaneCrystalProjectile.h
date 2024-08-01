// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraArcaneCrystalProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraArcaneCrystalProjectile : public AAuraProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Crystal")
	float OverlapIncreaseDamageMagnitude = .05f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arcane Crystal")
	float OverlapCooldown = .1f;

	void StartOverlapCooldown();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:

	bool bCanCauseOverlapDamage = true;

	FTimerHandle OverlapCooldownTimer;

	void OverlapCooldownTimerFinish();
};
