// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/SaveInterface.h"
#include "Interaction/HighLightInterface.h"
#include "BreakableActor.generated.h"

UCLASS()
class GASAURAGAME_API ABreakableActor : public AActor, public IAbilitySystemInterface, public ICombatInterface, public IHighlightInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	/* 保存接口开始 */
	FORCEINLINE virtual bool bShouldLoadTransform_Implementation() override { return false; };
	virtual void LoadActor_Implementation() override;
	/* 保存接口结束 */

	/* 高亮接口开始 */
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	virtual void SetMoveToDestination_Implementation(FVector& OutDestination) override;
	/* 高亮接口结束 */

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
	virtual AActor* GetAvatarActor_Implementation() override;

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	FOnDamageSignature OnDamageDelegate;
	/* 战斗接口结束 */

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnLoots();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ActorLevel = 1;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpluse);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bDead = false;

};
