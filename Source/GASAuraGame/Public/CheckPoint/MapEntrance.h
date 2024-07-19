// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
	
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/* 高亮接口 */
	virtual void HighlightActor_Implementation() override;
	/* 高亮接口结束 */

	/* 保存游戏接口 */
	virtual void LoadActor_Implementation() override;
	/* 保存游戏接口结束 */

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag = FName();

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
};
