-- Configuration du boss robot
boss = {
    -- Paramètres de base
    position = {x = 1500, y = 300},
    health = 10000,
    phase = 1,
    initial_position = {x = 1200, y = 300},
    
    -- Paramètres de tir
    shoot_cooldown = 2,
    shoot_interval = 3,
    bullet_speed = 0.3,
    
    -- Paramètres de mouvement
    movement_speed = 3.0,
    vertical_speed = 2.5,
    is_moving = true,
    
    -- États spéciaux
    is_laser_active = false,
    laser_duration = 2,
    laser_timer = 0,
    is_charging = false,
    charge_timer = 0,
    
    -- Timers et compteurs
    pattern_timer = 0,
    boss_timer = 0,
    next_shoot_time = 0,
    missile_count = 0,
    max_missiles = 4
}

-- Fonction pour les patterns de tir
local function shoot_pattern(pattern_type)
    local bullets = {}
    
    if pattern_type == "missile" then
        -- Tir de missile guidé
        local bullet = {
            x = boss.position.x - 50,
            y = boss.position.y,
            script = "scripts/bullets/simple_bullet.lua",
            type = 7,
            damage = 20,
            speed = boss.bullet_speed,
            angle = 180,
            collisionx = 90,
            collisiony = 90
        }
        table.insert(bullets, bullet)
        
    elseif pattern_type == "laser_beam" then
        -- Rayon laser (tir rectiligne rapide)
        local bullet = {
            x = boss.position.x - 30,
            y = boss.position.y,
            script = "scripts/bullets/simple_bullet.lua",
            type = 8,
            damage = 25,
            speed = boss.bullet_speed * 1.5,
            angle = 180,
            collisionx = 120,
            collisiony = 60
        }
        table.insert(bullets, bullet)
        
    elseif pattern_type == "shockwave" then
        -- Onde de choc en cercle
        for i = 0, 7 do
            local angle = i * 45
            local bullet = {
                x = boss.position.x,
                y = boss.position.y,
                script = "scripts/bullets/simple_bullet.lua",
                type = 9,
                damage = 15,
                speed = boss.bullet_speed * 0.8,
                angle = angle,
                collisionx = 80,
                collisiony = 80
            }
            table.insert(bullets, bullet)
        end
    end
    
    return bullets
end

-- Phase 1 : Attaques de base
local function phase1_behavior(current_time)
    local bullets = {}
    
    -- Mouvement mécanique (oscillation rigide)
    boss.position.y = boss.initial_position.y + math.sin(current_time * 0.3) * 100
    boss.position.x = math.max(1000, boss.initial_position.x + math.cos(current_time * 0.15) * 80)
    
    if boss.next_shoot_time <= current_time then
        bullets = shoot_pattern("missile")
        boss.next_shoot_time = current_time + boss.shoot_interval
    end
    
    return bullets
end

-- Phase 2 : Attaque laser
local function phase2_behavior(current_time)
    local bullets = {}
    
    -- Mouvement plus précis
    boss.position.y = boss.initial_position.y + math.sin(current_time * 0.5) * 150
    
    if boss.is_laser_active then
        boss.laser_timer = boss.laser_timer + 0.016
        
        -- Tirs laser réguliers
        if boss.laser_timer % 0.5 < 0.016 then
            bullets = shoot_pattern("laser_beam")
        end
        
        if boss.laser_timer >= boss.laser_duration then
            boss.is_laser_active = false
            boss.laser_timer = 0
            boss.next_shoot_time = current_time + 1.5
        end
    elseif boss.next_shoot_time <= current_time then
        boss.is_laser_active = true
        boss.laser_timer = 0
        boss.next_shoot_time = current_time + boss.laser_duration + 1.5
    end
    
    return bullets
end

-- Phase 3 : Mode berserk
local function phase3_behavior(current_time)
    local bullets = {}
    
    -- Mouvement agressif
    local t = current_time * 0.7
    boss.position.x = 1000 + math.cos(t) * 200
    boss.position.y = boss.initial_position.y + math.sin(t * 1.5) * 160
    
    if boss.next_shoot_time <= current_time then
        if boss.missile_count < boss.max_missiles then
            -- Rafale de missiles
            bullets = shoot_pattern("missile")
            boss.missile_count = boss.missile_count + 1
            boss.next_shoot_time = current_time + 0.4
        else
            -- Onde de choc après la rafale
            bullets = shoot_pattern("shockwave")
            boss.missile_count = 0
            boss.next_shoot_time = current_time + boss.shoot_interval
        end
    end
    
    return bullets
end

-- Fonction de mise à jour principale
function update_boss(delta_time)
    local bullets = {}
    
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
    boss.is_laser_active = false
    boss.laser_timer = 0
    boss.missile_count = 0
    boss_timer = os.clock()
end

function set_health(health)
    boss.health = health
end

function force_phase(phase)
    boss.phase = phase
    boss.health = (phase == 1 and 10000) or (phase == 2 and 6000) or (phase == 3 and 3000)
end