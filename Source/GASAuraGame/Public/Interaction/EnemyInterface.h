// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DecalActor.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASAURAGAME_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowEnemyArrow(UMaterialInterface* ArrowMaterialOverride);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HideEnemyArrow();
};
