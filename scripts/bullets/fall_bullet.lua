local speed = 0
initial_x = 0

initial_y = 0

local angle = 0

function init_bullet(x, y, _angle, _speed)
    initial_x = x
    initial_y = y
    angle = _angle
    speed = _speed
end

function update_bullet(deltaTime)
    position.x = position.x
    position.y = position.y + speed
end
