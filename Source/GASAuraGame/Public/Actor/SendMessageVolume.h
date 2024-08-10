// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/SaveInterface.h"
#include "Interaction/PlayerInterface.h"
#include "SendMessageVolume.generated.h"

UCLASS()
class GASAURAGAME_API ASendMessageVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASendMessageVolume();

	/* 保存接口开始 */
	// virtual void LoadActor_Implementation() override;
	/* 保存接口结束 */

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	int32 ApplicationTimes = 1;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> MessageEffectClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Send Message Volume")
	FGameplayTagContainer MessageTagContainer;
};
