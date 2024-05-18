// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "Input/AuraInputComponent.h"
#include "GameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Components/SplineComponent.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	FHitResult CursorHit;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const struct FInputActionValue& InputActionValue);

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* HoverActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponentBase> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponentBase* GetAuraASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThresHold = 0.3f;
	bool bAutoRunning = false;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float AutoRunAcceptanceRadius = 50.f;
	bool bTargeting = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
