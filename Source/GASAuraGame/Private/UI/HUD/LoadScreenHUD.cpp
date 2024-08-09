// Super Copyright


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint\UserWidget.h"

void ALoadScreenHUD::AddLoadScreenViewModel()
{
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);

	LoadScreenViewModel->InitializeLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
}

ULoadScreenWidget* ALoadScreenHUD::GetLoadScreenWidget()
{
	return LoadScreenWidget;
}

void ALoadScreenHUD::SetLoadScreenWidgetVisibility(ESlateVisibility Visibility)
{
	if (LoadScreenWidget)
	{
		LoadScreenWidget->SetVisibility(Visibility);
	}
}

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	AddLoadScreenViewModel();
	SetLoadScreenWidgetVisibility(ESlateVisibility::Hidden);

	OnLoadScreenWidgetInitializeFinish.Broadcast();
	/*
	* 读取存档的管线为：
	* 读取存档 -> 在Map中遍历所有已有存档并获取他们的信息 -> 将获取到的信息的变量设置到SlotWC的变量中(WC=WidgetController) ->
	* 初始化SlotWC(根据存档状态让其广播并切换到指定UI)
	* 
	* 因为MVVM的Property Path会在HUD的BeginPlay就开始初始化原因, 读取存档的操作已移到蓝图的 初始化操作 后
	*/
}
