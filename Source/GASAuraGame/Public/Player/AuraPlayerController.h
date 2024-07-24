// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "Input/AuraInputComponent.h"
#include "GameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Components/SplineComponent.h"
#include "UI/Widgets/DamageTextWidgetComponent.h"
#include "NiagaraSystem.h"
#include "Actor/MagicCircle.h"
#include "Interaction/HighlightInterface.h"
#include "UI/Widgets/FloatingTextWidgetComponent.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NoTargeting
};

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
	FORCEINLINE FHitResult GetCursorHitResult() const { return CursorHit; }

	UFUNCTION(Client, Reliable)
	void ClientShowDamageNumber(float DamageAmount, AActor* Target, bool bCriticalHit, bool bDamageValid);

	UFUNCTION(Client, Reliable)
	void ClientShowFloatingTextNumber(float Number, AActor* Target, const FString& TextPrefix = FString(""), const FString& TextSuffix = FString(""), FSlateColor ColorOverride = FLinearColor::Green);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle();
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	UFUNCTION(BlueprintCallable)
	void SetMagicCircleMaterial(UMaterialInterface* InMaterial);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	FHitResult CursorHit;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	FORCEINLINE void ShiftPressed() { bShiftKeyDown = true; }
	FORCEINLINE void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	void Move(const struct FInputActionValue& InputActionValue);

	void CursorTrace();
	AActor* LastTraceActor;
	AActor* TracingActor;

	void HighlightActor(AActor* Actor);
	void UnhighlightActor(AActor* Actor);

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
	ETargetingStatus TargetingStatus = ETargetingStatus::NoTargeting;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFloatingTextWidgetComponent> FloatingTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMagicCircle> MagicCircle;
	void UpdateMagicCircleLocation();
};
