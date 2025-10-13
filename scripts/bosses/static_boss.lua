-- Configuration du boss avec paramètres avancés
boss = {
    -- Paramètres de base
    position = {x = 800, y = 300},
    health = 9000,
    phase = 1,
    initial_position = {x = 0, y = 0},
    -- Paramètres de tir
    shoot_cooldown = 2,
    shoot_interval = 5,
    bullet_speed = 0.1,
    -- Paramètres de mouvement
    movement_speed = 2.5,
    target_x = 800,
    target_y = 300,
    is_moving = true,
    -- Paramètres de téléportation
    teleport_time = 5,
    teleport_cooldown = 10,
    can_teleport = true,
    -- Paramètres de phase
    phase3_start_time = 0,
    phase3_duration = 8,
    -- Paramètres de pattern
    current_pattern = 1,
    pattern_timer = 0,
    attack_cooldown = 0,
    angle = 0,
    -- États spéciaux
    is_enraged = false,
    rage_threshold = 0.3, -- 30% de vie restante
    invulnerable = false,
    boss_timer = 0,
    invulnerable_timer = 0,
    is_waiting = false,
    wait_timer = 0,
    current_moves = 0,
    initial_y = 0,
    teleport_count = 0,
    next_teleport_time = 0,
    next_shoot_time = 0,
    wait_end_time = nil -- Temps absolu où l'attente se termine
}
--current_time = 0.0
function calculate_delta_time()
    -- --current_time = current_time + 0.000001
    -- current_time = os.time()
    -- -- local elapsed_time = current_time - boss.boss_timer
    -- -- boss.boss_timer = current_time -- Mettre à jour le timer
    -- return current_time
    local current_time = os.time() -- ou une autre fonction de temps en secondes/microsecondes
    local elapsed_time = current_time - boss.boss_timer
    boss.boss_timer = current_time -- Mettre à jour le timer
    return elapsed_time
end


-- Patterns de tir avancés
local function shoot_pattern(deltaTime, pattern_type)
    local bullets = {}
    local script_path = "scripts/bullets/straight_bullet.lua"
    
    if pattern_type == "straight" then
        -- Tir droit simple
        local bullet = {
            x = boss.position.x,
            y = boss.position.y + 250,
            script = "scripts/bullets/straight_bullet.lua",
            type = 1,
            damage = 10,
            speed = 5,
            angle = 180,
            collisionx = 100,
            collisiony = 100
        }
        table.insert(bullets, bullet)
    
    elseif pattern_type == "spread" then
        -- Tir en éventail (3 balles)
        local angles = {-30, 0, 30}
        for _, angle in ipairs(angles) do
            local bullet = {
                x = boss.position.x,
                y = boss.position.y,
                script = "scripts/bullets/fall_bullet.lua",
                type = 2,
                damage = 8,
                speed = 5,
                angle = angle,
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
    
    elseif pattern_type == "circle" then
        -- Tir circulaire (8 balles)
        for i = 1, 8 do
            local angle = (i - 1) * 45
            local bullet = {
                x = boss.position.x,
                y = boss.position.y,
                script = "scripts/bullets/circle_bullet.lua",
                type = 3,
                damage = 6,
                speed = 5,
                angle = angle,
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
    
    elseif pattern_type == "fall_bullet" then
        -- Tir en spirale
        boss.angle = (boss.angle + 20) % 360
        local bullet = {
            x = boss.position.x,
            y = boss.position.y,
            script = "scripts/bullets/fall_bullet.lua",
            type = 1,
            damage = 12,
            speed = 5,
            angle = boss.angle,
            collisionx = 100,
            collisiony = 100
        }
        table.insert(bullets, bullet)
    end
    return bullets
end


-- Pattern de tir pour les balles alignées horizontalement
local function shoot_horizontal_pattern(pattern)
    local bullets = {}
    -- Positions des 4 balles alignées horizontalement
    local offsets = {-150, -50, 50, 150}  -- Espacement entre les balles

    if pattern == "straight" then
        for _, offset in ipairs(offsets) do
            local bullet = {
                x = boss.position.x + offset,
                y = boss.position.y,
                script = "scripts/bullets/straight_bullet.lua",
                type = 2,
                damage = 8,
                speed = 5,
                angle = 90,  -- Vers le bas
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
        return bullets

    elseif  pattern == "circle" then
        for _, offset in ipairs(offsets) do
            local bullet = {
                x = boss.position.x + offset,
                y = boss.position.y,
                script = "scripts/bullets/circle_bullet.lua",
                type = 3,
                damage = 8,
                speed = 5,
                angle = 90,  -- Vers le bas
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
        return bullets
    end
end

local function phase1_behavior(current_time)
    local bullets = {}
    local movement_limit = 4 -- Nombre de mouvements haut/bas avant arrêt

    -- Gestion de l'attente
    if boss.is_waiting then
        print("Current time = ", current_time)
        print("Boss wait end time = ", boss.wait_end_time)
        if current_time >= boss.wait_end_time then
            boss.is_waiting = false
            boss.current_moves = 0 -- Réinitialiser le compteur de mouvements
            boss.position.y = boss.initial_y -- Revenir à la position initiale
            print("Boss waiting finished. Resuming movement.")
        else
            return bullets -- Toujours en attente
        end
    end

    -- Gestion du mouvement vertical
    if boss.current_moves < movement_limit then
        if boss.moving_down then
            boss.position.y = boss.position.y + boss.movement_speed -- Descend
            if boss.position.y >= 500 then -- Limite basse atteinte
                boss.moving_down = false
                boss.current_moves = boss.current_moves + 1
                print("Reached lower limit. Moves: " .. boss.current_moves)
            end
        else
            boss.position.y = boss.position.y - boss.movement_speed -- Monte
            if boss.position.y <= 100 then -- Limite haute atteinte
                boss.moving_down = true
                boss.current_moves = boss.current_moves + 1
                print("Reached upper limit. Moves: " .. boss.current_moves)
            end
        end
    else
        -- Passer en mode attente après 4 mouvements
        boss.is_waiting = true
        boss.wait_end_time = current_time + 5.0 -- Attendre 5 secondes
        print("Boss entered waiting state. Will wait until:", boss.wait_end_time)
        --return bullets
    end

    print("Boss shoot cooldown: ", boss.shoot_cooldown)
    -- Gestion du tir
    if boss.shoot_cooldown <= current_time and not boss.is_waiting then
        boss.shoot_cooldown = current_time + boss.shoot_interval

        -- Tirer trois balles droites
        local straight_bullets = shoot_horizontal_pattern("straight")
        for _, bullet in ipairs(straight_bullets) do
            table.insert(bullets, bullet)
        end

        -- Tirer deux balles circulaires
        local circular_bullets = shoot_horizontal_pattern("circle")
        for _, bullet in ipairs(circular_bullets) do
            table.insert(bullets, bullet)
        end
    end

    return bullets
end

-- Phase 2 améliorée : Téléportations, patterns avancés, et retour à la position initiale
local function phase2_behavior(current_time)
    local bullets = {}

    -- Vérifiez si le boss peut téléporter
    if boss.next_teleport_time <= current_time then
        if boss.teleport_count < 4 then
            -- Points de téléportation stratégiques
            local teleport_points = {
                {x = 200, y = 200},
                {x = 1400, y = 200},
                {x = 800, y = 100},
                {x = 200, y = 500},
                {x = 1400, y = 500}
            }
            
            -- Téléportation
            local point = teleport_points[math.random(#teleport_points)]
            boss.position.x = point.x
            boss.position.y = point.y
            boss.teleport_count = boss.teleport_count + 1 -- Incrémenter le compteur
            boss.next_teleport_time = current_time + 3 -- Temps jusqu'à la prochaine téléportation
            boss.next_shoot_time = current_time + 0.5 -- Tir rapide après téléportation
            print("Boss teleported to:", point.x, point.y)
        else
            -- Retourner à la position initiale
            boss.position.x = boss.initial_position.x
            boss.position.y = boss.initial_position.y
            boss.teleport_count = 0 -- Réinitialiser le compteur de téléportation
            boss.next_teleport_time = current_time + 3 -- Temps avant la prochaine téléportation
            print("Boss returned to initial position:", boss.initial_position.x, boss.initial_position.y)
        end
    end

    -- Vérifiez si le boss peut tirer
    if boss.next_shoot_time <= current_time then
        print("Boss Phase 2: Complex attack pattern")
        boss.next_shoot_time = current_time + boss.shoot_interval * 0.8 -- Plus rapide qu'en phase 1
        
        -- Alterne entre cercle et spirale
        if boss.current_pattern == 1 then
            bullets = shoot_pattern(current_time, "circle")
            boss.current_pattern = 2
        else
            bullets = shoot_pattern(current_time, "spiral")
            boss.current_pattern = 1
        end
    end

    return bullets
end


-- Phase 3 améliorée : Mode enragé et patterns mortels
local function phase3_behavior(current_time)
    local bullets = {}

    -- Initialisation de la phase 3
    if boss.phase3_start_time == 0 then
        boss.phase3_start_time = current_time
        boss.bullet_speed = boss.bullet_speed * 1.5 -- Augmente la vitesse des balles
    end

    -- Mouvement plus agressif (utilisation du temps courant pour les oscillations)
    local elapsed_time = current_time - boss.phase3_start_time
    boss.position.x = 800 + math.cos(elapsed_time * 2) * 400
    boss.position.y = 300 + math.sin(elapsed_time * 3) * 200

    -- Vérification de l'état enragé
    if boss.health / 9000 <= boss.rage_threshold and not boss.is_enraged then
        boss.is_enraged = true
        boss.shoot_interval = boss.shoot_interval * 0.6 -- Tirs plus rapides en mode enragé
        boss.bullet_speed = boss.bullet_speed * 1.2 -- Balles plus rapides
        print("Boss is enraged!")
    end

    -- Gestion des tirs
    if boss.next_shoot_time <= current_time then
        print("Boss Phase 3: Ultimate attack pattern")
        boss.next_shoot_time = current_time + boss.shoot_interval

        -- Combine plusieurs patterns en mode enragé
        if boss.is_enraged then
            local pattern1 = shoot_pattern(current_time, "circle")
            local pattern2 = shoot_pattern(current_time, "fall_bullet")

            for _, bullet in ipairs(pattern2) do
                table.insert(pattern1, bullet)
            end
            bullets = pattern1
        else
            -- Alterne entre tous les patterns
            local patterns = {"straight", "fall_bullet"}
            bullets = shoot_pattern(current_time, patterns[boss.current_pattern])
            boss.current_pattern = (boss.current_pattern % #patterns) + 1
        end
    end

    return bullets
end

-- Fonction de mise à jour principale améliorée
function update_boss(delta_time)
    local bullets = {}

    -- Détermination de la phase
    if boss.health > 6000 then
        boss.phase = 1
    elseif boss.health > 3000 then
        boss.phase = 2
    elseif boss.health > 0 then
        boss.phase = 3
    else
        boss.phase = 0
        return {} -- Boss vaincu
    end

    print("Boss position: " .. boss.position.x .. ", " .. boss.position.y)
    print("Boss phase: " .. boss.phase)
    print("Boss health:" .. boss.health)

    -- Exécution du comportement de la phase
    if boss.phase == 1 then
        bullets = phase1_behavior(delta_time)
    elseif boss.phase == 2 then
        bullets = phase2_behavior(delta_time)
    elseif boss.phase == 3 then
        bullets = phase3_behavior(delta_time)
    end
    return bullets or {}
end

-- Initialisation améliorée
function init_boss(posX, posY, health)
    boss.position = {x = posX, y = posY}
    boss.health = health or 9000
    boss.phase = 3
    boss.teleport_cooldown = 0
    boss.shoot_cooldown = boss.shoot_interval
    boss.phase3_start_time = 0
    boss.current_pattern = 1
    boss.pattern_timer = 0
    boss.angle = 0
    boss.initial_position.x = posX
    boss.initial_position.y = posY
    boss.initial_y = posY
    boss.is_enraged = false
    boss.invulnerable = false
    boss.invulnerable_timer = 0
    boss.bullet_speed = 300
    boss_timer = os.clock()
end

function set_health(health)
    boss.health = health
end

function force_phase(phase)
    boss.phase = phase
    boss.health = (phase == 1 and 9000) or (phase == 2 and 6000) or (phase == 3 and 3000)
end
