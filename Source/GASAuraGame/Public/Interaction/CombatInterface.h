// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "CombatInterface.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASAURAGAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCombatLevel();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	virtual void Die();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatarActor();
};
