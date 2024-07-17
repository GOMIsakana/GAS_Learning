// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Components/SphereComponent.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ACheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()
	
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/* 保存游戏 接口 */
	virtual bool bShouldLoadTransform_Implementation() const  { return false; };

	virtual void LoadActor_Implementation() override;
	/* 保存游戏 接口结束 */

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* GlowMaterialInst);

	void HandleCheckPointGlowEffect();

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
