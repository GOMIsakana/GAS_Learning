// Super Copyright


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCP)
{
	PlayerController = WCP.PlayerController;
	PlayerState = WCP.PlayerState;
	AbilitySystemComponent = WCP.AbilitySystemComponent;
	AttributeSet = WCP.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{

}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
