
-- Variables locales pour l'état
local speed = 4
local can_shoot = true
local shoot_cooldown = 0.3  -- Fréquence de tir ajustée
local shoot_interval = 0.3  -- Réduit l'intervalle pour plus de rapidité

-- Fonction pour créer des balles et vérifier s'il est temps de tirer
function update_bullet(deltaTime)
    -- Réduire le cooldown en fonction du temps écoulé
    shoot_cooldown = shoot_cooldown - deltaTime

    --print("Delta Time coll : ", shoot_cooldown - deltaTime * 100)
    --print("shoot cooldown", shoot_cooldown)
    
    -- Si le cooldown est terminé, tirer une nouvelle salve
    if shoot_cooldown <= 0 then
        -- Déterminer le nombre de balles à tirer
        local num_bullets = 1  -- Vous pouvez ajuster ce nombre aussi
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
                speed = 6
            }
            table.insert(spawned_bullets, bullet)
        end

        -- Réinitialiser le cooldown
        shoot_cooldown = shoot_interval + math.random() * 0.2

        -- Retourner les balles créées pour cette salve
        return spawned_bullets
    end

    -- Si aucune balle n'est tirée, retourner une liste vide
    return {}
end

local startY = 0
local startX = 0

function init_enemy(x, y)
    startX = x
    startY = y
end

function update(deltaTime)
    -- Déplace l'ennemi vers la gauche
    position.x = position.x - speed
    position.y = position.y

end
