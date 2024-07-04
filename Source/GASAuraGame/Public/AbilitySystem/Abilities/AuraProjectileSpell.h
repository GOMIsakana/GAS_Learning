// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/AuraProjectile.h"
#include "Net/UnrealNetwork.h"
#include "AuraProjectileSpell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileSpawnFinishedSignature);

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "ProjectileSpell")
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetDescriptionNextLevel(int32 Level) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumProjectile = 1;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileSpawnFinishedSignature OnProjectileSpawnFinishedSignature;

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
