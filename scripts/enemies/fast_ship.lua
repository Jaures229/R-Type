-- Variables de base
local speed = 4
local can_shoot = false
local shoot_cooldown = 5
local shoot_interval = 10

local startY = 0
local startX = 0


-- Fonction pour créer des balles et vérifier s'il est temps de tirer
function update_bullet(deltaTime)
    -- Réduire le cooldown en fonction du temps écoulé
    shoot_cooldown = shoot_cooldown - deltaTime *10
    --print("This is shoot colldown", shoot_cooldown)

    if can_shoot == true then
        if shoot_cooldown <= 0 then
            local num_bullets = math.random(1, 3)
            local spawned_bullets = {}
    
            for i = 1, num_bullets do
                local script_path = "scripts/bullets/straight_bullet.lua"
                local bullet = {
                    x = position.x - 100,
                    y = position.y,
                    script = script_path,
                    type = 1,
                    damage = 10,
                    collisionx = 100,
                    collisiony = 100,
                    angle = 0,
                    speed = 5,
                }
                table.insert(spawned_bullets, bullet)
            end
    
            -- Réinitialiser le cooldown
            shoot_cooldown = shoot_interval
    
            -- Retourner les balles créées pour cette salve
            return spawned_bullets
        end        
    end


    -- Si aucune balle n'est tirée, retourner une liste vide
    return {}
end

function init_enemy(x, y)
    startX = x
    startY = y
end

-- function update(deltaTime)
--     position.x = position.x - speed
--     position.y = position.y

-- end

local time = 0

function update(deltaTime)
    local amplitude = 100     -- Amplitude du zigzag (hauteur)
    local frequency = 2       -- Fréquence du changement de direction
    local direction = (math.floor(time * frequency) % 2) * 2 - 1  -- Alterne entre 1 et -1
    
    -- Déplacement horizontal constant
    position.x = position.x - speed
    
    -- Mouvement vertical en zigzag
    position.y = position.y + speed * direction
    
    -- Limites verticales pour le zigzag
    if math.abs(position.y - startY) >= amplitude then
        position.y = startY + amplitude * direction
        time = time + 0.5    -- Force le changement de direction
    end
    
    -- Mise à jour du temps
    time = time + deltaTime
end