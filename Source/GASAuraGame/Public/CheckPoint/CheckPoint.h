// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Components/SphereComponent.h"
#include "CheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()
	
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* GlowMaterialInst);

	void HandleCheckPointGlowEffect();

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
