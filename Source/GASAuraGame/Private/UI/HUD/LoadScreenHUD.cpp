// Super Copyright


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint\UserWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	if (LoadScreenViewModelClass)
	{
		LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
		LoadScreenViewModel->InitializeLoadSlots();
	}

	if (LoadScreenWidgetClass)
	{
		LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
		LoadScreenWidget->AddToViewport();
		LoadScreenWidget->BlueprintInitializeWidget();
	}
}
