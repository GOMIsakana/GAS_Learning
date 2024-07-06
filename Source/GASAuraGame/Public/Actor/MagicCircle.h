// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "MagicCircle.generated.h"

UCLASS()
class GASAURAGAME_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterialInterface* InMaterial);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> MagicCircleMaterial;

public:	

};
