// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

UCLASS()
class GASAURAGAME_API ABreakableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnLoots();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ActorLevel = 1;
protected:
	virtual void BeginPlay() override;

};
