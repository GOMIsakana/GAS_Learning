// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class GASAURAGAME_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();

protected:
	virtual void BeginPlay() override;

};
