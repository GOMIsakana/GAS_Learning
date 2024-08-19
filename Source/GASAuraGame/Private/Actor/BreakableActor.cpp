// Super Copyright


#include "Actor/BreakableActor.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void ABreakableActor::LoadActor_Implementation()
{
	if (bDead)
	{
		SetActorHiddenInGame(true);
	}
}

void ABreakableActor::HighlightActor_Implementation()
{
}

void ABreakableActor::UnhighlightActor_Implementation()
{
}

void ABreakableActor::SetMoveToDestination_Implementation(FVector& OutDestination)
{
	
}

UAbilitySystemComponent* ABreakableActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

float ABreakableActor::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	OnDamageDelegate.Broadcast(DamageTaken);

	return DamageTaken;
}

void ABreakableActor::Die_Implementation(const FVector& DeathImpluse)
{
	OnDeath.Broadcast(this);
}

bool ABreakableActor::IsDead_Implementation() const
{
	return false;
}

ECharacterClass ABreakableActor::GetCharacterClass_Implementation()
{
	return ECharacterClass::BreakableActor;
}

FOnASCRegistered& ABreakableActor::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& ABreakableActor::GetOnDeathDelegate()
{
	return OnDeath;
}

FOnDamageSignature& ABreakableActor::GetOnDamageDelegate()
{
	return OnDamageDelegate;
}
