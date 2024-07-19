// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "Components/BoxComponent.h"
#include "Actor/AuraEnemySpawnPoint.h"
#include "AuraEnemySpawnVolume.generated.h"

UCLASS()
class GASAURAGAME_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();

	/* 存档接口开始 */
	virtual void LoadActor_Implementation() override;
	/* 存档接口结束 */

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnActorPoints;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UFUNCTION()
	void BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
