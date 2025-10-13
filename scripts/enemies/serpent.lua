local speed = 3
local can_shoot = true
local shoot_cooldown = 0.5
local shoot_interval = 0.1

-- Fonction pour créer des balles et vérifier s'il est temps de tirer
function update_bullet(deltaTime)
    -- Réduire le cooldown en fonction du temps écoulé
    shoot_cooldown = shoot_cooldown - deltaTime

    --print("shoot colldown", shoot_cooldown)
    -- Si le cooldown est terminé, tirer une nouvelle salve
    if shoot_cooldown <= 0 then
        -- Déterminer le nombre de balles à tirer
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
                speed = 7,
            }
            table.insert(spawned_bullets, bullet)
        end

        -- Réinitialiser le cooldown
        shoot_cooldown = shoot_interval + math.random() * 0.3

        -- Retourner les balles créées pour cette salve
        return spawned_bullets
    end

    -- Si aucune balle n'est tirée, retourner une liste vide
    return {}
end


local startY = 0
local startX = 0
local centerX = 0
local centerY = 0

function init_enemy(x, y)
    startX = x
    startY = y
    centerX = x
    centerY = y
end

-- function update(deltaTime)
--     position.x = position.x - speed
--     position.y = position.y
-- end

local time = 0

function update(deltaTime)
    local radius = 50         -- Rayon du cercle
    local rotationSpeed = 60   -- Vitesse de rotation
    
    -- Calcul de la position sur le cercle
    position.x = centerX + radius * math.cos(time * rotationSpeed)
    position.y = centerY + radius * math.sin(time * rotationSpeed)
    
    -- Déplacement constant vers la gauche
    centerX = centerX - speed
    
    -- Mise à jour du temps
    time = time + deltaTime
end