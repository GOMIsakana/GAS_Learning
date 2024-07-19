// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Character/AuraEnemy.h"
#include "AuraEnemySpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:

	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass EnemyCharacterClass = ECharacterClass::Warrior;
};
