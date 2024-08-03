
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>哥布林弹弓手</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>视力比较好的哥布林, 使用弹弓进行远程攻击</>
<Default>石头砸人和长矛戳人哪个更痛呢? 大概是一样痛吧</>]]
end

return M