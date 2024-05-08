// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();

	/* 敌人接口 */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* 敌人接口结尾 */

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
};
