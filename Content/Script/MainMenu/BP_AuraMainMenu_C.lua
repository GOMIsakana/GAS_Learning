local M = UnLua.Class()

local UGameplayStatics = UE.UGameplayStatics
local UNiagaraFunctionLibrary = UE.UNiagaraFunctionLibrary
local UWidgetBlueprintLibrary = UE.UWidgetBlueprintLibrary
local EMouseLockMode = UE.EMouseLockMode
local ENCPoolMethod = UE.ENCPoolMethod
local ESlateVisibility = UE.ESlateVisibility


function M:OnPlayButtonClicked()
    UNiagaraFunctionLibrary.SpawnSystemAtLocation(self, self.DissappearSystem, self:K2_GetActorLocation(), self:K2_GetActorRotation(), self:GetActorScale3D(), true, true, ENCPoolMethod.None, true)
    self.AuraMesh:SetHiddenInGame(true, true)
    self.MainMenuWidget:SetVisibility(ESlateVisibility.Hidden)
    self.BP_LoadScreenHUD:SetLoadScreenWidgetVisibility(ESlateVisibility.Visible)
end

function M:OnLoadMenuQuitButtonClicked()
    self.AuraMesh:SetHiddenInGame(false, true)
    self.MainMenuWidget:SetVisibility(ESlateVisibility.Visible)
    self.BP_LoadScreenHUD:SetLoadScreenWidgetVisibility(ESlateVisibility.Hidden)
end

function M:OnLoadScreenWidgetInitializeFinish()
    self.BP_LoadScreenHUD:GetLoadScreenWidget().QuitButton.Button.OnClicked:Add(self, self.OnLoadMenuQuitButtonClicked)   -- 绑定到加载菜单Widget的退出游戏按钮上
end

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)

    local PlayerController = UGameplayStatics.GetPlayerController(self, 0)
    self.BP_LoadScreenHUD = PlayerController:GetHUD()
    self.MainMenuWidget = NewObject(self.MainMenuWidgetClass, self, nil, "UI.MainMenu.WBP_MainMenu_C")
    self.MainMenuWidget:AddToViewport();
    UWidgetBlueprintLibrary.SetInputMode_UIOnlyEx(PlayerController, self.MainMenuWidget, EMouseLockMode.DoNotLock, false)
    self.MainMenuWidget.PlayButton.Button.OnClicked:Add(self, self.OnPlayButtonClicked)   -- 绑定到主菜单Widget的开始游戏按钮上
    self.BP_LoadScreenHUD.OnLoadScreenWidgetInitializeFinish:Add(self, self.OnLoadScreenWidgetInitializeFinish)  -- 等待HUD初始化完成, 再绑定加载菜单的按钮的委托
    PlayerController.bShowMouseCursor = true
end

return M