local M = UnLua.Class()

local UKismetMathLibrary = UE.UKismetMathLibrary

function M:ReceiveTick(DeltaTime)
    self.Overridden.ReceiveTick(self, DeltaTime)

    local Owner = self:GetOwner()
    if (Owner ~= nil) then
        local SelfLocation = self:K2_GetActorLocation()    -- 因为自己是附着在目标玩家身上的, 所以获取自己的位置等于获取了目标玩家的位置
        local OwnerLocation = Owner:K2_GetActorLocation()
        local ToOwnerRotation = UKismetMathLibrary.MakeRotFromX((SelfLocation - OwnerLocation))   -- 计算到目标距离
        ToOwnerRotation.Roll = 0
        ToOwnerRotation.Pitch = -90
        ToOwnerRotation.Yaw = ToOwnerRotation.Yaw - 90
        self:K2_SetActorRotation(ToOwnerRotation, true)
    end
end

return M