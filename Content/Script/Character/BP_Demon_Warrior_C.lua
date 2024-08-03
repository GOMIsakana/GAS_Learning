
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>恶魔</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>恶魔的一种, 拥有强健的尾巴, 能够把人抽飞、抽到晕厥</>]]
end

return M