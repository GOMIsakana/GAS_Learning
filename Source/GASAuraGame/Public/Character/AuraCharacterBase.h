// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class GASAURAGAME_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	// UE的新标准, 尽量用TObject
	UPROPERTY(EditAnywhere, Category = "武器")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
