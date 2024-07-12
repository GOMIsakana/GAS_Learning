// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widgets/LoadScreenWidget.h"
#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "LoadScreenHUD.generated.h"

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "LoadScreen")
	TSubclassOf<ULoadScreenWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "LoadScreen")
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
protected:

	virtual void BeginPlay() override;
};
