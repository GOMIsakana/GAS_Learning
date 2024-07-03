// Super Copyright


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;

}

void UDebuffNiagaraComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
}

void UDebuffNiagaraComponent::Deactivate()
{
	Super::Deactivate();
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* OwnerCombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		OwnerASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (OwnerCombatInterface)
	{
		OwnerCombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
			}
		);
	}

	if (OwnerCombatInterface)
	{
		OwnerCombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
