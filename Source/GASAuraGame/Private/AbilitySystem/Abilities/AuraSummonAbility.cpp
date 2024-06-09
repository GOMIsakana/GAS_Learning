// Super Copyright


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = NumberOfMinions == 1 ? SpawnPositionRotator : SpawnPositionRotator / (NumberOfMinions - 1);

	const FVector LeftRotator = Forward.RotateAngleAxis(-SpawnPositionRotator / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumberOfMinions; i++)
	{
		const FVector Direction = LeftRotator.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenedSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult GroundHitResult;
		GetWorld()->LineTraceSingleByChannel(GroundHitResult, ChosenedSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenedSpawnLocation + FVector(0.f, 0.f, -400.f), ECollisionChannel::ECC_WorldStatic);
		
		if (GroundHitResult.bBlockingHit)
		{
			ChosenedSpawnLocation = GroundHitResult.ImpactPoint;
		}

		SpawnLocations.Add(ChosenedSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	if (MinionsClasses.Num() > 0)
	{
		return MinionsClasses[FMath::Rand() % MinionsClasses.Num()];
	}
	return TSubclassOf<APawn>();
}
