
local M = UnLua.Class()

function M:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    self.ActorInfoTitle = "<Title>恶魔</>"
    self.ActorInfoDescription = [[<Damage>敌人</>
<Default>恶魔的一种, 也会让人被打飞, 然后晕厥</>
<Default>但它的方法是扔石头(为什么是扔石头呢? 为什么呢?)</>]]
end

return M