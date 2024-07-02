// Super Copyright


#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.Location;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerInfo()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	if (OwnerCharacter && OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* WeaponMesh = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			FVector TraceStart = WeaponMesh->GetSocketLocation(FName("TipSocket"));
			TArray<AActor*> ActorToIgnore;
			ActorToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			UKismetSystemLibrary::SphereTraceSingle(
				this,
				TraceStart,
				BeamTargetLocation,
				5.f,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ActorToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true
			);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MouseHitActor);
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLifePlayerWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		FMath::Max(AdditionalTargetSearchRadius, 350.f + GetAbilityLevel() * 50.f),
		MouseHitLocation
	);

	int32 EffectiveTargetNum = FMath::Max(NumOfAdditionalTarget, GetAbilityLevel());

	UAuraAbilitySystemLibrary::GetClosestTargets(EffectiveTargetNum, OverlappingActors, OutAdditionalTargets, MouseHitLocation);
}
