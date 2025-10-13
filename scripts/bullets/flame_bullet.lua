local speed = 5
local angle = 0
--local position = {x = 0, y = 0}

function init_bullet(x, y, _angle, _speed)
    --position.x = x
    --position.y = y
    angle = _angle
    speed = 0.1
end

function update_bullet(delta_time)
    -- Utilise les fonctions cosinus et sinus pour le mouvement directionnel
    local wave = math.sin(delta_time * 10) * 5
    
    -- Calcul du déplacement en fonction de l'angle
    position.x = position.x + math.cos(math.rad(angle)) * speed * delta_time
    position.y = position.y - math.sin(math.rad(angle)) * speed * delta_time
    
    --return position
end
