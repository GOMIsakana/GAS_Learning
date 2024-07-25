// Super Copyright


#include "AbilitySystem/Abilities/AuraFireNado.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraFireNado::SpawnTornado(FVector SpawnLocation)
{
	if (TornadoClass)
	{
		int32 EffectiveSpawnAmount = FMath::Max(TornadoAmount, GetAbilityLevel() / 3);
		for (int i = 0; i < EffectiveSpawnAmount; i++)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			FVector SpawnScale(RadialDamageOuterRadius / 200.f);
			SpawnTransform.SetScale3D(SpawnScale);
			AAuraTornado* Tornado = GetWorld()->SpawnActorDeferred<AAuraTornado>(
				TornadoClass,
				SpawnTransform,
				GetAvatarActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			FDamageEffectParams Params = MakeDamageEffectParamasFromClassDefaults();
			float AdditionalMagicalDamage = GetAdditionalAttributeValueByName(FName("FireNado.Hit"), GetAbilityLevel()) / 100.f * GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UAuraAttributeSet::GetMagicalDamageAttribute());
			Params.BaseDamage = Params.BaseDamage + AdditionalMagicalDamage;
			Params.MinRadialDamage = MinRadialDamagePercentage / 100.f * Params.BaseDamage;
			Tornado->DamageEffectParams = Params;
			Tornado->SetLifeSpan(TornadoDuration);
			Tornado->SetDealDamageFrequency(DealDamageFrequency);
			Tornado->SetDealingDamage(true);
			Tornado->FinishSpawning(SpawnTransform);
		}
	}
	TornadoSpawnComponentDelegate.Broadcast();
}
