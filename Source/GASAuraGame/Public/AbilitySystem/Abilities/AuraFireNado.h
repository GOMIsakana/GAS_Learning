// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/AuraTornado.h"
#include "AuraAbilityTypes.h"
#include "AuraFireNado.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTornadoSpawnComponentSignature);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraFireNado : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnTornado(FVector SpawnLocation);

	UPROPERTY(BlueprintAssignable)
	FTornadoSpawnComponentSignature TornadoSpawnComponentDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireNado")
	TSubclassOf<AAuraTornado> TornadoClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireNado")
	int32 TornadoAmount = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float TornadoDuration = 5.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tornado")
	float DealDamageFrequency = 0.5f;
};
