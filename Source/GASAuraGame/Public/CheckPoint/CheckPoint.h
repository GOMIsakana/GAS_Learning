// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Components/SphereComponent.h"
#include "Interaction/SaveInterface.h"
#include "Interaction/HighlightInterface.h"
#include "GASAuraGame/GASAuraGame.h"
#include "GameplayEffect.h"
#include "CheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ACheckPoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/* 保存游戏接口 */
	FORCEINLINE virtual bool bShouldLoadTransform_Implementation() override { return false; };

	virtual void LoadActor_Implementation() override;
	/* 保存游戏接口结束 */

	/* 高亮接口 */
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	virtual void SetMoveToDestination_Implementation(FVector& OutDestination) override;
	/* 高亮接口结束 */

	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* GlowMaterialInst);

	UFUNCTION(BlueprintCallable)
	void HandleCheckPointGlowEffect();

	UFUNCTION(BlueprintCallable)
	void HandleReachedGameplayEffect(AActor* TargetActor);

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bBindOverlapCallback = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> ReachedGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Level = 1;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToLocationComponent;
private:
};
