// Super Copyright


#include "Actor/AuraProjectile.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(Capsule);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraProjectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

