-- Configuration du boss dragon
boss = {
    -- Paramètres de base
    position = {x = 1500, y = 300},
    health = 10000,
    phase = 1,
    initial_position = {x = 1200, y = 300},
    
    -- Paramètres de tir
    shoot_cooldown = 2,
    shoot_interval = 4,
    bullet_speed = 0.1,
    
    -- Paramètres de mouvement
    movement_speed = 2.5,
    vertical_speed = 2.0,
    is_moving = true,
    
    -- États spéciaux
    is_breathing_fire = false,
    breath_duration = 3,
    breath_timer = 0,
    is_charging = false,
    charge_timer = 0,
    
    -- Timers et compteurs
    pattern_timer = 0,
    boss_timer = 0,
    next_shoot_time = 0,
    fireball_count = 0,
    max_fireballs = 3
}

-- Fonction pour les patterns de tir
local function shoot_pattern(pattern_type)
    local bullets = {}
    
    if pattern_type == "fireball" then
        -- Tir de boule de feu simple
        local bullet = {
            x = boss.position.x - 50,
            y = boss.position.y,
            script = "scripts/bullets/fireball_bullet.lua",
            type = 4,
            damage = 15,
            speed = boss.bullet_speed,
            angle = 0,
            collisionx = 100,
            collisiony = 100
        }
        table.insert(bullets, bullet)
        
    elseif pattern_type == "dragon_breath" then
        -- Souffle de feu continu (multiple petites balles)
        for i = -2, 2 do
            local bullet = {
                x = boss.position.x - 30,
                y = boss.position.y + (i * 20),
                script = "scripts/bullets/flame_bullet.lua",
                type = 5,
                damage = 8,
                speed = boss.bullet_speed * 1.2,
                angle = 180,
                collisionx = 80,
                collisiony = 80
            }
            table.insert(bullets, bullet)
        end
        
    elseif pattern_type == "flame_wave" then
        -- Vague de flammes en arc
        for i = -3, 3 do
            local _angle = i * 45
            local bullet = {
                x = boss.position.x - 40,
                y = boss.position.y + (i * 40),
                script = "scripts/bullets/wave_bullet.lua",
                type = 6,
                damage = 12,
                speed = 0.3,
                angle = _angle,
                collisionx = 90,
                collisiony = 90
            }
            table.insert(bullets, bullet)
        end
    end
    
    return bullets
end

-- Phase 1 : Attaques de base
local function phase1_behavior(current_time)
    local bullets = {}
    
    -- Mouvement de tête de dragon (oscillation douce)
    boss.position.y = boss.initial_position.y + math.sin(current_time * 0.4) * 150
    boss.position.x = math.max(1000, boss.initial_position.x + math.cos(current_time * 0.2) * 100)
    
    if boss.next_shoot_time <= current_time then
        -- Alterne entre boules de feu simples
        bullets = shoot_pattern("fireball")
        boss.next_shoot_time = current_time + boss.shoot_interval
    end
    
    return bullets
end

-- -- Phase 2 : Souffle du dragon
-- local function phase2_behavior(current_time)
--     local bullets = {}
    
--     -- Mouvement plus agressif
--     boss.position.y = boss.initial_position.y + math.sin(current_time * 0.6) * 200
    
--     if boss.is_breathing_fire then
--         boss.breath_timer = boss.breath_timer + 0.016
--         if boss.breath_timer >= boss.breath_duration then
--             boss.is_breathing_fire = false
--             boss.breath_timer = 0
--             boss.next_shoot_time = current_time + 2
--         else
--             -- Souffle continu pendant la durée
--             bullets = shoot_pattern("dragon_breath")
--         end
--     elseif boss.next_shoot_time <= current_time then
--         -- Commence une nouvelle séquence de souffle
--         boss.is_breathing_fire = true
--         boss.breath_timer = 0
--         boss.next_shoot_time = current_time + boss.breath_duration + 2
--     end
    
--     return bullets
-- end

-- Phase 2 : Souffle du dragon
local function phase2_behavior(current_time)
    local bullets = {}
    
    -- Mouvement plus agressif
    boss.position.y = boss.initial_position.y + math.sin(current_time * 0.6) * 200
    
    if boss.is_breathing_fire then
        boss.breath_timer = boss.breath_timer + 0.016
        
        -- Calcul de l'intervalle pour les 5 souffles
        local breath_interval = boss.breath_duration / 4  -- 3 secondes divisées en 4 intervalles = 5 souffles
        
        -- Vérifier si c'est le moment de tirer un souffle
        if boss.breath_timer % breath_interval < 0.016 then  -- Utilisation du modulo pour les intervalles réguliers
            bullets = shoot_pattern("dragon_breath")
        end
        
        -- Fin de la séquence de souffle
        if boss.breath_timer >= boss.breath_duration then
            boss.is_breathing_fire = false
            boss.breath_timer = 0
            boss.next_shoot_time = current_time + 2
        end
    elseif boss.next_shoot_time <= current_time then
        -- Commence une nouvelle séquence de souffle
        boss.is_breathing_fire = true
        boss.breath_timer = 0
        boss.next_shoot_time = current_time + boss.breath_duration + 2
    end
    
    return bullets
end

-- Phase 3 : Fureur du dragon
local function phase3_behavior(current_time)
    local bullets = {}
    
    -- Mouvement erratique
    local t = current_time * 0.8
    boss.position.x = 1000 + math.cos(t) * 150
    boss.position.y = boss.initial_position.y + math.sin(t * 2) * 180
    
    if boss.next_shoot_time <= current_time then
        if boss.fireball_count < boss.max_fireballs then
            -- Rafale de boules de feu
            bullets = shoot_pattern("fireball")
            boss.fireball_count = boss.fireball_count + 1
            boss.next_shoot_time = current_time + 0.5
        else
            -- Vague de flammes après la rafale
            bullets = shoot_pattern("flame_wave")
            boss.fireball_count = 0
            boss.next_shoot_time = current_time + boss.shoot_interval
        end
    end
    
    return bullets
end

-- Fonction de mise à jour principale
function update_boss(delta_time)
    local bullets = {}

    -- print("Boss position y:", boss.position.x)
    -- print("Boss position y:", boss.position.y)
    -- Détermination de la phase basée sur la santé
    if boss.health > 6000 then
        boss.phase = 1
    elseif boss.health > 3000 then
        boss.phase = 2
    else
        boss.phase = 3
    end
    
    -- Exécution du comportement de la phase
    if boss.phase == 1 then
        bullets = phase1_behavior(delta_time)
    elseif boss.phase == 2 then
        bullets = phase2_behavior(delta_time)
    elseif boss.phase == 3 then
        bullets = phase3_behavior(delta_time)
    end
    
    return bullets
end

-- Initialisation
function init_boss(posX, posY, health)
    boss.position = {x = posX or 1500, y = posY or 300}
    boss.health = health or 10000
    boss.phase = 1
    boss.initial_position.x = posX or 1200
    boss.initial_position.y = posY or 300
    boss.next_shoot_time = 0
    boss.is_breathing_fire = false
    boss.breath_timer = 0
    boss.fireball_count = 0
    boss_timer = os.clock()
end

function set_health(health)
    boss.health = health
end

function force_phase(phase)
    boss.phase = phase
    boss.health = (phase == 1 and 10000) or (phase == 2 and 6000) or (phase == 3 and 3000)
end

-- init_boss()

-- k = 0
-- while true do
--     k = k+1
--     update_boss(k)    
-- end