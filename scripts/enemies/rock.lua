
local speed = 3 -- Vitesse de déplacement de l'ennemi
local shoot_cooldown = 0.5 -- Cooldown initial (en secondes)
local shoot_interval = 0.2 -- Intervalle de tir (en secondes)
local startY = 0
local startX = 0


-- Fonction pour créer des balles et vérifier s'il est temps de tirer
function update_bullet(deltaTime)
    -- Réduire le cooldown en fonction du temps écoulé
    shoot_cooldown = shoot_cooldown - deltaTime

    --print("Cooldown restant :", shoot_cooldown)

    if shoot_cooldown <= 0 then
        -- Déterminer le nombre de balles à tirer
        local num_bullets = math.random(1, 3) -- Entre 1 et 3 balles
        local spawned_bullets = {}

        for i = 1, num_bullets do
            local script_path = "scripts/bullets/fall_bullet.lua"
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

            local bullet2 = {
                x = position.x - 100,
                y = position.y,
                script = "scripts/bullets/up_bullet.lua",
                type = 1,
                damage = 10,
                collisionx = 100,
                collisiony = 100,
                angle = 0,
                speed = 5,
            }
            table.insert(spawned_bullets, bullet2)
        end

        -- Réinitialiser le cooldown pour le prochain tir
        shoot_cooldown = shoot_interval + math.random() * 0.2 -- Ajout d'une petite variance (0 à 0.2s)
        return spawned_bullets
    end

    -- Si aucune balle n'est tirée, retourner une liste vide
    return {}
end


function init_enemy(x, y)
    startX = x
    startY = y
end

-- Fonction principale de mise à jour
function update(deltaTime)
    -- Déplacer l'ennemi
    position.x = position.x - speed

    -- Mettre à jour les balles (et simuler les tirs)
    local bullets = update_bullet(deltaTime)
end
