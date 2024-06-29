// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTags.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:

	UDebuffNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DebuffTag;

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

protected:
	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
