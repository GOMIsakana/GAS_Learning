// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Data/CharacterClassInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/* 敌人接口 */

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
	/* 敌人接口结尾 */

	/* 高亮接口 */
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	virtual void SetMoveToDestination_Implementation(FVector& OutDestination) override;
	/* 高亮接口结尾 */

	/* 战斗接口 */
	virtual int32 GetCombatLevel_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual void SetIsBeingShock_Implementation(bool bInIsBeingShock) override;
	/* 战斗接口结尾 */

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnEnemyHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnEnemyMaxHealthChanged;

	UFUNCTION()
	void OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	virtual void Die(const FVector& DeathImpluse) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void OnStunTagChanged(FGameplayTag ReceivedTag, int32 NewCount) override;

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharacterClass;

	virtual void InitializeDefaultAttribute() const override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

private:
	void BindCallback();
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;
};
