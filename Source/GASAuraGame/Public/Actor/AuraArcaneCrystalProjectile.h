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

protected:
	virtual void BeginPlay() override;

	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
