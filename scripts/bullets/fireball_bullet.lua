local speed = 5
local angle = 0

initial_x = 0
initial_y = 0


function init_bullet(x, y, _angle, _speed)
    initial_x = x
    initial_y = y
    angle = _angle
    speed = 0.2
end

function update_bullet(delta_time)
    -- Mise à jour de la position
    position.x = position.x - math.cos(math.rad(angle)) * speed * delta_time
    position.y = position.y - math.sin(math.rad(angle)) * speed * delta_time
end
