// Super Copyright


#include "Actor/AuraEnemySpawnPoint.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	if (EnemyClass == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetEnemyLevel(EnemyLevel);
	Enemy->SetCharacterClass(EnemyCharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
