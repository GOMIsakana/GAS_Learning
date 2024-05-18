// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AuraProjectile.generated.h"

UCLASS()
class GASAURAGAME_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;
};
