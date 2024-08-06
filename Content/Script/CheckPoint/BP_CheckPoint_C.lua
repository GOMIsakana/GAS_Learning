local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>检查点</>"
    self.ActorInfoDescription = [[<ArcaneDamage>检查点</>
<Default>靠近即可激活, 并自动保存游戏。</>
<Default>并恢复所有生命值和法力值</>]]
end

return M