local speed = 2
local rotation_speed = 3
local triangle_radius = 30

local initial_x = 0
local initial_y = 0
local base_y = 0
local current_rotation = 0

function init_bullet(x, y)
    initial_x = x
    initial_y = y
    base_y = y
    current_rotation = math.random() * 360  -- Angle de départ aléatoire
end

function update_bullet(deltaTime)
    -- Déplacement vers la gauche
    position.x = position.x - speed
    
    -- Calcul du mouvement triangulaire
    current_rotation = current_rotation + rotation_speed
    
    -- Calcul de l'offset triangulaire pour la position Y
    local angle_rad = math.rad(current_rotation)
    local y_offset = math.sin(angle_rad * 3) * triangle_radius
    
    -- Application du mouvement triangulaire
    position.y = base_y + y_offset
end