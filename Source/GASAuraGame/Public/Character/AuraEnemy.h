// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
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
class GASAURAGAME_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/* 敌人接口 */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* 敌人接口结尾 */

	/* 战斗接口 */
	virtual int32 GetCombatLevel() override;
	/* 战斗接口结尾 */

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnEnemyHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnEnemyMaxHealthChanged;

	UFUNCTION()
	void OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	virtual void Die() override;

protected:
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
