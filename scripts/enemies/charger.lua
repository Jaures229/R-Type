
-- Variables locales pour l'état
local speed = 5
local can_shoot = true
local shoot_cooldown = 0.5
local shoot_interval = 0.1


-- Fonction pour créer des balles et vérifier s'il est temps de tirer
function update_bullet(deltaTime)
    -- Réduire le cooldown en fonction du temps écoulé
    shoot_cooldown = shoot_cooldown - deltaTime

    --print("This is shoot colldown", shoot_cooldown)
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
        shoot_cooldown = shoot_interval + math.random() * 0.1

        -- Retourner les balles créées pour cette salve
        return spawned_bullets
    end

    -- Si aucune balle n'est tirée, retourner une liste vide
    return {}
end

local startY = 0
local startX = 0
local time = 0       -- Compteur de temps

function init_enemy(x, y)
    startX = x
    startY = y
end

local state = nil    -- État actuel du mouvement
-- function update(deltaTime)
--     -- Variables pour le mouvement sinusoïdal
--     local amplitude = 100 -- Hauteur du mouvement
--     local frequency = 10  -- Vitesse de l'oscillation
    
--     -- Déplacement horizontal
--     position.x = position.x - speed
    
--     -- Mouvement sinusoïdal vertical
--     position.y = startY + amplitude * math.sin(time * frequency)
    
--     -- Mise à jour du temps
--     time = time + deltaTime
-- end

function update(deltaTime)
    local diveSpeed = 3       -- Vitesse de plongée
    local maxDiveAngle = 60   -- Angle maximum de plongée en degrés
    local diveDistance = 200  -- Distance de plongée
    
    -- État de l'ennemi (descente ou montée)
    if not state then
        state = "dive"        -- État initial: plongée
        startY = position.y   -- Mémoriser la position de départ
    end
    
    -- Déplacement horizontal constant
    position.x = position.x - speed
    
    -- Gestion du mouvement de piqué-remonté
    if state == "dive" then
        -- Phase de descente
        position.y = position.y + diveSpeed
        
        -- Vérifier si on a atteint la profondeur maximale
        if position.y >= startY + diveDistance then
            state = "climb"
        end
    else
        -- Phase de remontée
        position.y = position.y - diveSpeed
        
        -- Revenir à la position initiale et recommencer
        if position.y <= startY then
            position.y = startY
            state = "dive"
        end
    end
end