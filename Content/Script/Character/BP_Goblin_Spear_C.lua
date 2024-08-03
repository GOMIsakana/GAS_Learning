
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林长矛兵</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>随处可见的哥布林, 但要小心它手上的长矛, 那可不是闹着玩的</>]]
end

return M