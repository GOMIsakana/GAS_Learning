// Super Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widgets/LoadScreenWidget.h"
#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "LoadScreenHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadScreenWidgetInitializeFinish);

/**
 * 
 */
UCLASS()
class GASAURAGAME_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnLoadScreenWidgetInitializeFinish OnLoadScreenWidgetInitializeFinish;

	UFUNCTION(BlueprintCallable)
	void AddLoadScreenViewModel();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULoadScreenWidget* GetLoadScreenWidget();

	UFUNCTION(BlueprintCallable)
	void SetLoadScreenWidgetVisibility(ESlateVisibility Visibility);

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
