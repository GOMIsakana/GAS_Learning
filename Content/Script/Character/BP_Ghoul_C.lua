
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>食尸鬼</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>骇人的生物, 体型比哥布林大上不少, 但不知为何只吃你却不吃哥布林</>]]
end

return M