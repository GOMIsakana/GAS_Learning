// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	EAP_ApplyOnStartOverlap UMETA(DisplayName = "开始重叠时"),
	EAP_ApplyOnEndOverlap UMETA(DisplayName = "结束重叠时"),

	EAP_DoNotApply UMETA(DisplayName = "不应用")
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	ERP_RemoveOnStartOverlap UMETA(DisplayName = "开始重叠时"),
	ERP_RemoveOnEndOverlap UMETA(DisplayName = "结束重叠时"),

	ERP_DoNotRemove UMETA(DisplayName = "不应用")
};

UCLASS()
class GASAURAGAME_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	virtual void Tick(float DeltaTime) override;

	void DoTickRotation(float DeltaTime);

	void DoTickFloating(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	bool bShouldRotation = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	bool bShouldFloating = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	float RotationRate = 120.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	float ZAxisFloatingDistance = 50.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Effects")
	float ZAxisFloatingSpeed = 4.f;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "bApplySuccessful") bool ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::ERP_RemoveOnEndOverlap;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	int32 InfiniteEffectRemovalStack = -1;

	// TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> InfiniteEffectHandlesMap;
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> InfiniteEffectHandlesMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Apply Effect")
	float ActorLevel = 1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Apply Effect")
	TArray<FName> AffectableTag;

	float SineCount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float SineResetClampInPI = 2.f;
};
