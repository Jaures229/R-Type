-- local speed = 7

-- function update_bullet(deltaTime)
--     position.x = position.x
--     position.y = position.y + speed
-- end


local radius = 50 -- Rayon du cercle
local angle = 0   -- Angle en radians
local speed = 5   -- Vitesse angulaire (radians par seconde)

local initial_x = 0
local initial_y = 0
local radial_speed = 50
local radius = 0
local radial_speed = 50

function init_bullet(x, y, _angle, _speed)
    initial_x = x
    initial_y = y
    angle = _angle
    speed = _speed
end

function update_bullet(deltaTime)
    position.x = position.x - speed
    position.y = position.y
end
