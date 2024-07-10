// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	FDamageEffectParams ExplodeDamageParam;

	UPROPERTY(BlueprintReadOnly)
	AActor* FlyBackActor;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnHit() override;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartFlyOutTimeline();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartFlyBackTimeline();

private:
};
