// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/BackpackInterface.h"
#include "Components/SphereComponent.h"
#include "DropItemActor.generated.h"

UCLASS()
class GASAURAGAME_API ADropItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADropItemActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBackpackItem ItemInfo;
protected:
};
