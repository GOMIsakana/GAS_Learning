local M = UnLua.Class()

-- 因为WidgetControllerSet属于函数, 所以不要加Receive到函数名前面
function M:WidgetControllerSet()
    self.Overridden.WidgetControllerSet(self)
    self.BP_OverlayWidgetController.OnReceivedTitleMessageDelegate:Add(self, self.OnReceivedTitleMessage)
    self.BP_OverlayWidgetController.MultiMessageWidgetRowDelegate:Add(self, self.HandleMultiMessageWidgetRowDelegate)
end

function M:OnReceivedTitleMessage(Title, SubTitle)
    self.MapTitleNotice:ShowTitleNotice(Title, SubTitle)
end

local function SyncHandleMultiMessageRow(Self, MessageWidget, MessageRow)
    UE.UKismetSystemLibrary.Delay(Self, MessageRow.DelayToThisMessage)
    RowWidget = NewObject(MessageWidget, Self:GetOwningPlayer(), nil, "UI.Overlay.SubWidget.WBP_EffectMessage_C")
    Self.PickupMessageBox:AddChild(RowWidget)
    RowWidget:SetImageAndText(MessageRow.Image, MessageRow.Message, "")
    RowWidget:StartAutoDestroy(MessageRow.RemoveDelay)
end

function M:HandleMultiMessageWidgetRowDelegate(Row)
    for i = 1, Row.Messages:Length() do
        local MessageRow = Row.Messages:Get(i)
        coroutine.resume(coroutine.create(SyncHandleMultiMessageRow), self, Row.MessageWidget, MessageRow)
    end
end

return M