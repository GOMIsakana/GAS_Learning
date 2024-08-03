
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林萨满</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>比较聪明的哥布林, 能够使用老家秘传的绝技召唤出恶魔</>]]
end

return M