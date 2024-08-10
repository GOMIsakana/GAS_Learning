// Super Copyright


#include "Actor/SendMessageVolume.h"

ASendMessageVolume::ASendMessageVolume()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASendMessageVolume::BeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	if (ApplicationTimes <= 0) return;
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		if (MessageEffectClass)
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(MessageEffectClass, 1.f, EffectContextHandle);
			EffectSpecHandle.Data->AppendDynamicAssetTags(MessageTagContainer);
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
		ApplicationTimes--;
	}
}

