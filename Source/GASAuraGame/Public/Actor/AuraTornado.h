// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "AuraTornado.generated.h"

UCLASS()
class GASAURAGAME_API AAuraTornado : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraTornado();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetDealingDamage(bool bShouldDealDamage);

	void DealDamageAtSelfLocation();

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams DamageEffectParams;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDealDamageFrequency(float InDealDamageFrequency) { DealDamageFrequency = InDealDamageFrequency; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void MoveToTarget(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	bool bFollowNearestEnemy = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float FollowRadius = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float DealDamageFrequency = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float TornadoStepLength = 10.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float TornadoMovementSpeed = 1.f;

private:
	FTimerHandle DealDamageTimer;
	TArray<AActor*> ActorToIgnoreDealDamage;
	TArray<AActor*> NearestTargets;
	TObjectPtr<AActor> TargetActor = nullptr;
};
