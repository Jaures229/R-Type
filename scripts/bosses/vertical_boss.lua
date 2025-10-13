-- Configuration du boss
boss = {
    -- Paramètres de base
    position = {x = 1500, y = 300},
    health = 8000, -- Plus de santé
    phase = 1,
    initial_position = {x = 1200, y = 300},
    -- Paramètres de tir
    shoot_cooldown = 2.5, -- Tirs plus fréquents
    shoot_interval = 4,
    bullet_speed = 15, -- Balles plus rapides
    -- Paramètres de mouvement
    movement_speed = 3.0, -- Plus rapide
    vertical_speed = 2.0,
    is_moving = true,
    -- Paramètres de pattern
    current_pattern = 1,
    angle = 0,
    -- États
    is_charging = false,
    charge_timer = 0,
    charge_duration = 1.5, -- Charge plus rapide
    warning_shot = false,
    -- Timers
    pattern_timer = 0,
    boss_timer = 0,
    next_shoot_time = 0,
    -- Nouveaux paramètres
    triangle_rotation = 0,
    burst_count = 0,
    max_bursts = 3
}

-- Fonction pour les patterns de tir
local function shoot_pattern(pattern_type)
    local bullets = {}
    
    if pattern_type == "warning" then
        -- Tir d'avertissement plus rapide
        local bullet = {
            x = boss.position.x - 50,
            y = boss.position.y,
            script = "scripts/bullets/straight_bullet.lua",
            type = 1,
            damage = 8,
            speed = boss.bullet_speed * 0.8,
            angle = 180,
            collisionx = 100,
            collisiony = 100
        }
        table.insert(bullets, bullet)
        
    elseif pattern_type == "spread" then
        -- Tir en éventail plus large
        for i = -2, 2 do
            local bullet = {
                x = boss.position.x - 50,
                y = boss.position.y,
                script = "scripts/bullets/straight_bullet.lua",
                type = 2,
                damage = 10,
                speed = boss.bullet_speed + (math.abs(i) * 20), -- Vitesses variables
                angle = 180 + (i * 12),
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
        
    elseif pattern_type == "vertical" then
        -- Tirs verticaux plus denses
        for i = -3, 3 do
            local bullet = {
                x = boss.position.x - 30,
                y = boss.position.y + (i * 40),
                script = "scripts/bullets/fall_bullet.lua",
                type = 3,
                damage = 8,
                speed = boss.bullet_speed * 0.9,
                angle = 180,
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end

    elseif pattern_type == "triangle" then
        -- Configuration du triangle
        local radius = 40  -- Distance du centre aux pointes du triangle
        local angles = {0, 120, 240}  -- Angles pour former un triangle équilatéral
        
        -- Création des 3 balles positionnées en triangle
        for _, angle in ipairs(angles) do
            -- Conversion en radians et calcul des offsets
            local rad = math.rad(angle)
            local offsetX = math.cos(rad) * radius
            local offsetY = math.sin(rad) * radius
            
            -- Création de la balle
            local bullet = {
                x = boss.position.x - 50 + offsetX,  -- -50 pour décaler légèrement du boss
                y = boss.position.y + offsetY,
                script = "scripts/bullets/straight_bullet.lua", -- On utilise le même script que les balles normales
                type = 2,
                damage = 12,
                speed = boss.bullet_speed,
                angle = 180, -- Vers la gauche
                collisionx = 100,
                collisiony = 100
            }
            table.insert(bullets, bullet)
        end
    end
    -- elseif pattern_type == "triangle" then
    --     -- Nouveau pattern en triangle
    --     local angles = {0, 120, 240}
    --     local radius = 30
    --     boss.triangle_rotation = (boss.triangle_rotation + 5) % 360
        
    --     for _, baseAngle in ipairs(angles) do
    --         local angle = baseAngle + boss.triangle_rotation
    --         local rad = math.rad(angle)
    --         local offsetX = math.cos(rad) * radius
    --         local offsetY = math.sin(rad) * radius
            
    --         local bullet = {
    --             x = boss.position.x - 50 + offsetX,
    --             y = boss.position.y + offsetY,
    --             script = "scripts/bullets/triangle_bullet.lua",
    --             type = 1,
    --             damage = 12,
    --             speed = boss.bullet_speed * 1.1,
    --             angle = 180,
    --             collisionx = 100,
    --             collisiony = 100
    --         }
    --         table.insert(bullets, bullet)
    --     end
    -- end
    
    return bullets
end

-- Phase 1 : Plus aggressive
local function phase1_behavior(current_time)
    local bullets = {}
    
    -- Mouvement plus complexe
    boss.position.y = boss.initial_position.y + math.sin(current_time * 0.8) * 180
    boss.position.x = math.max(1000, boss.position.x - math.sin(current_time * 0.3) * 2)
    
    if boss.next_shoot_time <= current_time then
        if not boss.warning_shot then
            bullets = shoot_pattern("warning")
            boss.warning_shot = true
            boss.next_shoot_time = current_time + 1.0
        else
            local patterns = {"spread", "triangle"}
            bullets = shoot_pattern(patterns[boss.current_pattern])
            boss.current_pattern = boss.current_pattern % 2 + 1
            boss.warning_shot = false
            boss.next_shoot_time = current_time + boss.shoot_interval
        end
    end
    
    return bullets
end

-- Phase 2 : Plus intense
local function phase2_behavior(current_time)
    local bullets = {}
    
    -- Mouvement plus erratique
    boss.position.y = boss.initial_position.y + math.sin(current_time * 1.2) * 220
    boss.position.x = 1000 + math.cos(current_time * 0.5) * 150
    
    if boss.next_shoot_time <= current_time then
        if boss.burst_count < boss.max_bursts then
            bullets = shoot_pattern(boss.is_charging and "triangle" or "vertical")
            boss.is_charging = not boss.is_charging
            boss.next_shoot_time = current_time + 0.8
            boss.burst_count = boss.burst_count + 1
        else
            boss.burst_count = 0
            boss.next_shoot_time = current_time + boss.shoot_interval
        end
    end
    
    return bullets
end

-- Phase 3 : Mortelle
local function phase3_behavior(current_time)
    local bullets = {}
    
    -- Mouvement complexe en 8
    local t = current_time * 0.7
    boss.position.x = 1000 + math.cos(t) * 120
    boss.position.y = boss.initial_position.y + math.sin(t * 2) * 180
    
    if boss.next_shoot_time <= current_time then
        -- Combine plusieurs patterns
        local pattern1 = shoot_pattern("triangle")
        local pattern2 = shoot_pattern(boss.current_pattern == 1 and "spread" or "vertical")
        
        -- Fusionne les patterns
        for _, bullet in ipairs(pattern2) do
            table.insert(pattern1, bullet)
        end
        
        bullets = pattern1
        boss.current_pattern = boss.current_pattern % 2 + 1
        boss.next_shoot_time = current_time + boss.shoot_interval * 0.7
    end
    
    return bullets
end

-- Fonction de mise à jour principale
function update_boss(delta_time)
    local bullets = {}
    
    if boss.health > 5000 then
        boss.phase = 1
    elseif boss.health > 2500 then
        boss.phase = 2
    else
        boss.phase = 3
    end
    
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
    boss.health = health or 8000
    boss.phase = 1
    boss.initial_position.x = posX or 1200
    boss.initial_position.y = posY or 300
    boss.next_shoot_time = 0
    boss.current_pattern = 1
    boss.is_charging = false
    boss.charge_timer = 0
    boss.warning_shot = false
    boss.triangle_rotation = 0
    boss.burst_count = 0
    boss_timer = os.clock()
end

function set_health(health)
    boss.health = health
end

function force_phase(phase)
    boss.phase = phase
    boss.health = (phase == 1 and 8000) or (phase == 2 and 5000) or (phase == 3 and 2500)
end