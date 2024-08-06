local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>信标</>"
    self.ActorInfoDescription = [[<ArcaneDamage>检查点</>
<Default>靠近即可激活, 并自动保存游戏, 但出生点不会保存至信标处</>]]
end

return M