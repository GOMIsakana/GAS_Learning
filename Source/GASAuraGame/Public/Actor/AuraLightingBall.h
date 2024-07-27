// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraLightingBall.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraLightingBall : public AAuraProjectile
{
	GENERATED_BODY()
	
	// 每次Tick扫描周围内若干个目标, 将其添加到目标列表里, 在目标不超出范围的情况下对其发动闪电链造成持续伤害

	// TODO: 将特效转换成GameplayCue执行

public:
	AAuraLightingBall();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting Ball")
	float MaxConnectingDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting Ball")
	float TargetSearchingDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting Ball")
	float DealDamageFrequency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting Ball")
	int32 MaxTargetAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting Ball")
	TObjectPtr<UNiagaraSystem> LightingChainSystem;

	UFUNCTION(BlueprintCallable)
	void RefreshTargetArray();

	UFUNCTION(BlueprintCallable)
	void DealDamage();

	UFUNCTION()
	void SetDealingDamage(bool bShouldDealDamage);

	UFUNCTION()
	void StartGameplayCue();
	UFUNCTION()
	void EndGameplayCue();

	UPROPERTY()
	FGameplayTag LightingChainsGameplayCueTag;

	UPROPERTY()
	FGameplayCueParameters LightingChainsCueParameters;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void LifeSpanExpired() override;

	void InitializeLightingChainTargets();

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> Targets;

	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> LightingChainTargets;

private:
	FTimerHandle DealDamageTimer;
};
