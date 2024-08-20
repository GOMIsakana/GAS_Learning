// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "BGMSoundComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASAURAGAME_API UBGMSoundComponent : public UMediaSoundComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchSoundMedia(UMediaSource* NewMedia, float BGMVolumeMultiplier);
};
