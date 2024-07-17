// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveTag;

protected:
	virtual void BeginPlay() override;

	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);

	void ActivateIfEquipped(UAuraAbilitySystemComponentBase* AuraASC);
};
