// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "BreakableActor.generated.h"

UCLASS()
class GASAURAGAME_API ABreakableActor : public AActor, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	/* 能力接口开始 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* 能力接口结束 */

	/* 战斗接口开始 */
	virtual void Die_Implementation(const FVector& DeathImpluse) override;
	virtual bool IsDead_Implementation() const override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	FOnDamageSignature OnDamageDelegate;
	/* 战斗接口结束 */

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnLoots();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ActorLevel = 1;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDead = false;

};
