-- Taille de l'écran
screenWidth = 1920
screenHeight = 1080

-- Liste des types d'ennemis disponibles
enemy_types = {
    {type = "FastShip", script = "scripts/enemies/fast_ship.lua", collision_x = 100, collision_y = 100, health = 10},
    {type = "Charger", script = "scripts/enemies/charger.lua", collision_x = 100, collision_y = 100, health = 10},
    {type = "Tank", script = "scripts/enemies/tank.lua", collision_x = 100, collision_y = 100, health = 100},
    {type = "Serpent", script = "scripts/enemies/serpent.lua", collision_x = 100, collision_y = 100, health = 200},
    {type = "Rock", script = "scripts/enemies/rock.lua", collision_x = 500, collision_y = 200, health = 200}
}

-- Boss disponibles
boss_types = {
    {type = "StaticBoss", script = "scripts/bosses/static_boss.lua", collision_x = 509, collision_y = 509, health = 9000, nb_type = 1, tag = "Blue_Nebular", posX = 1420, posY = 240},
    {type = "VerticalBoss", script = "scripts/bosses/vertical_boss.lua", collision_x = 983, collision_y = 113, health = 8000, nb_type = 2, tag = "Green_Nebular", posX = 1500, posY = 300},
    {type = "DragonHead", script = "scripts/bosses/DragonHead.lua", collision_x = 100, collision_y = 101, health = 10000, nb_type = 3, tag = "Red_Nebular", posX = 1500, posY = 300},
    {type = "Megatron", script = "scripts/bosses/Megatron.lua", collision_x = 458, collision_y = 324, health = 10000, nb_type = 4, tag = "Yellow_Nebular", posX = 1500, posY = 300}
}

-- Plateformes disponibles
platform_types = {
    {type = "Solid", width = 128, height = 55, collision = true},
    {type = "PassThrough", width = 150, height = 15, collision = true},
    {type = "Decorative", width = 300, height = 30, collision = false}
}

-- Paramètres de génération
wave_interval = 5           -- Temps entre les vagues (secondes)
boss_interval = 5            -- Générer un boss toutes les N vagues
enemies_per_wave = 5         -- Nombre d'ennemis par vague
enemy_spawn_area = {x_min = screenWidth + 100, x_max = screenWidth + 500, y_min = 100, y_max = 500}
platform_spawn_area = {x_min = 100, x_max = screenWidth - 100, y_min = 100, y_max = screenHeight - 100}
wave_cooldown = 3 -- 3 secondes

-- Suivi de la progression
current_wave = 0
last_wave_time = 0
boss_active = false          -- Indique si un boss est actuellement actif


-- Liste des ennemis et plateformes générés
spawned_enemies = {}
spawned_platforms = {}


local last_boss_time = 0
local boss_warning_time = 5
active_enemies_count = 0
MAX_ACTIVE_ENEMIES = 25
min_enemies_per_wave = 7  -- Minimum d'ennemis par vague pour garder l'intérêt du jeu

-- Fonction d'initialisation du niveau
function init_level(level)
    current_level = level
    current_wave = 0
    spawned_enemies = {}
    spawned_platforms = {}
    boss_active = false
    print("Level " .. current_level .. " initialized!")

end

-- Fonction pour générer des plateformes
function generate_platforms(count)
    for i = 1, count do
        local platform_type = platform_types[math.random(1, #platform_types)]
        local spawn_x = math.random(platform_spawn_area.x_min, platform_spawn_area.x_max)
        local spawn_y = math.random(platform_spawn_area.y_min, platform_spawn_area.y_max)

        table.insert(spawned_platforms, {
            type = platform_type.type,
            width = platform_type.width,
            height = platform_type.height,
            collision = platform_type.collision,
            x = spawn_x,
            y = spawn_y
        })
    end
    print(count .. " platforms generated.")
end


function generate_triangle_formation(center_x, center_y, num_enemies, spacing)
    local enemies = {}
    for i = 1, 2 do
        local row = math.ceil(math.sqrt(i))
        local col = i - (row - 1) * row
        local offset_x = (col - 1) * 100
        local offset_y = (row - 1) * 100
        -- local spawn_x = center_x + (i - 1) * spacing  -- Espacement horizontal
        -- local spawn_y = center_y - (i - 1) * spacing  -- Espacement vertical

        table.insert(enemies, {
            type = "FastShip",
            script = "scripts/enemies/fast_ship.lua",
            x = center_x + offset_x,
            y = center_y + offset_y,
            collision_x = 100,
            collision_y = 100,
            health = 10
        })
    end
    print("Generated triangle formation.")
    return enemies
end

function generate_circle_formation(center_x, center_y, num_enemies, radius)
    local enemies = {}
    for i = 1, 4 do
        local angle = (i - 1) * (2 * math.pi / num_enemies)  -- Angle pour chaque ennemi
        local spawn_x = center_x + radius * math.cos(angle)
        local spawn_y = center_y + radius * math.sin(angle)

        table.insert(enemies, {
            type = "FastShip",
            script = "scripts/enemies/fast_ship.lua",
            x = spawn_x,
            y = spawn_y,
            collision_x = 100,
            collision_y = 100,
            health = 10
        })
    end
    print("Generated circle formation.")
    return enemies
end

function generate_straight_line_formation(start_x, start_y, num_enemies, spacing)
    local enemies = {}
    for i = 1, num_enemies do
        local spawn_x = start_x + (i - 1) * spacing  -- Espacement horizontal
        local spawn_y = start_y

        table.insert(enemies, {
            type = "Charger",
            script = "scripts/enemies/charger.lua",
            x = spawn_x,
            y = spawn_y,
            collision_x = 205,
            collision_y = 18,
            health = 20
        })
    end
    print("Generated straight line formation.")
    return enemies
end


function generate_v_formation(center_x, center_y, num_enemies, spacing)
    local enemies = {}
    local half = math.floor(6 / 2)
    
    for i = 1, half do
        local offset = i * spacing
        -- Ennemis à gauche
        table.insert(enemies, {
            type = "Serpent",
            script = "scripts/enemies/serpent.lua",
            x = center_x - offset,
            y = center_y + i * spacing,
            collision_x = 230,
            collision_y = 27,
            health = 10
        })

        -- Ennemis à droite
        table.insert(enemies, {
            type = "Serpent",
            script = "scripts/enemies/serpent.lua",
            x = center_x + offset,
            y = center_y + i * spacing,
            collision_x = 230,
            collision_y = 27,
            health = 10
        })
    end
    print("Generated V formation.")
    return enemies
end

function generate_z_formation(start_x, start_y, num_enemies, spacing)
    local enemies = {}

    local positions = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0},
        {0, 1}, {2, 1},
        {0, 2}, {3, 2},
        {0, 3}, {1, 3}, {2, 3}, {3, 3}
    }
    for i, pos in ipairs(positions) do
        local offset_x = pos[1] * 150 -- Espacement horizontal
        local offset_y = pos[2] * 150 -- Espacement vertical
        table.insert(enemies, {
            type = "Tank",
            script = "scripts/enemies/tank.lua",
            x = start_x + offset_x,
            y = start_y + offset_y,
            collision_x = 196,
            collision_y = 50,
            health = 20
        })
    end
    print("Generated Z formation.")
    return enemies
end

function generate_spiral_formation(center_x, center_y, num_enemies, radius)
    local enemies = {}
    local angle_increment = 0.1
    local radius_increment = radius / num_enemies

    for i = 1, num_enemies do
        local angle = i * angle_increment
        local spawn_x = center_x + radius * math.cos(angle)
        local spawn_y = center_y + radius * math.sin(angle)

        table.insert(enemies, {
            type = "FastShip",
            script = "scripts/enemies/fast_ship.lua",
            x = spawn_x,
            y = spawn_y,
            collision_x = 263,
            collision_y = 30,
            health = 10
        })

        radius = radius - radius_increment
    end
    print("Generated spiral formation.")
    return enemies
end

function generate_wave_formation(start_x, start_y, num_enemies, wave_amplitude, wave_frequency)
    local enemies = {}
    for i = 1, num_enemies do
        local offset_x = (i - 1) * 100
        local offset_y = wave_amplitude * math.sin(i * wave_frequency)  -- Génère l'effet de vague

        table.insert(enemies, {
            type = "Charger",
            script = "scripts/enemies/charger.lua",
            x = start_x + offset_x,
            y = start_y + offset_y,
            collision_x = 205,
            collision_y = 18,
            health = 10
        })
    end
    print("Generated wave formation.")
    return enemies
end

function generate_block_formation(center_x, center_y, num_enemies, spacing)
    local enemies = {}
    local rows = math.ceil(math.sqrt(num_enemies)) -- Calculer le nombre de lignes pour former un carré
    local cols = math.ceil(num_enemies / rows)     -- Calculer le nombre de colonnes

    -- Positionner les ennemis en une grille compacte centrée autour de `center_x` et `center_y`
    local start_x = center_x - ((cols - 1) * spacing / 2)
    local start_y = center_y - ((rows - 1) * spacing / 2)

    for i = 0, rows - 1 do
        for j = 0, cols - 1 do
            local enemy_x = start_x + j * spacing
            local enemy_y = start_y + i * spacing
            if #enemies < num_enemies then
                table.insert(enemies, { x = enemy_x, y = enemy_y,
                type = "Rock",
                script = "scripts/enemies/rock.lua",
                health = 30,
                collision_x = 522,
                collision_y = 132,

            })
            end
        end
    end

    return enemies
end

function generate_diamond_formation(center_x, center_y, num_enemies, spacing)
    local enemies = {}
    local half = math.floor(4 / 2) -- Diviser les ennemis en deux parties pour le losange

    -- Générer la moitié supérieure du losange
    for i = 0, half do
        local count = i * 2 + 1 -- Nombre d'ennemis sur la ligne
        local start_x = center_x
        for j = 0, count - 1 do
            table.insert(enemies, {
                x = start_x + j * spacing,
                y = center_y - i * spacing,
                type = "LZE",
                script = "scripts/enemies/tank.lua",
                health = 20,
                collision_x = 266,
                collision_y = 32
            })
        end
    end

    return enemies
end

-- Fonction de génération de vague avec gestion du nombre d'ennemis actifs
function generate_wave()
    current_wave = current_wave + 1
    print("Generating wave " .. current_wave)

    local enemies = {}
    local formation_choice = math.random(1, 12)
    
    -- Calculer combien d'ennemis nous pouvons ajouter
    local available_slots = MAX_ACTIVE_ENEMIES - active_enemies_count
    
    -- Si nous n'avons pas assez de place pour une nouvelle vague minimale, ne pas générer
    if available_slots < min_enemies_per_wave then
        --print("Not enough space for new wave. Current active enemies: " .. active_enemies_count)
        return { enemies = {} }
    end

    -- Ajuster enemies_per_wave en fonction de l'espace disponible
    local adjusted_enemies_per_wave = math.min(enemies_per_wave, available_slots)
    
    -- Définir les paramètres pour les formations
    local start_x = screenWidth
    local center_x = screenWidth
    local center_y = math.random(100, screenHeight - 100)
    local spacing = 150
    local radius = 300
    local wave_amplitude = 100
    local wave_frequency = 0.3

    -- Choisir la formation avec le nombre ajusté d'ennemis
    if formation_choice == 1 then
        enemies = generate_circle_formation(center_x, center_y, adjusted_enemies_per_wave, radius)
    elseif formation_choice == 2 then
        enemies = generate_straight_line_formation(start_x, center_y, adjusted_enemies_per_wave, spacing)
    elseif formation_choice == 3 then
        enemies = generate_v_formation(center_x, center_y, adjusted_enemies_per_wave, spacing)
    elseif formation_choice == 4 then
        enemies = generate_z_formation(start_x, center_y, adjusted_enemies_per_wave, spacing)
    elseif formation_choice == 5 then
        enemies = generate_spiral_formation(center_x, center_y, adjusted_enemies_per_wave, radius)
    elseif formation_choice == 6 then
        enemies = generate_wave_formation(start_x, center_y, adjusted_enemies_per_wave, wave_amplitude, wave_frequency)
    elseif formation_choice == 7 then
        enemies = generate_triangle_formation(start_x, center_y, adjusted_enemies_per_wave, spacing)
    elseif formation_choice == 9 then
        enemies = generate_block_formation(center_x, center_y, adjusted_enemies_per_wave, spacing)
    elseif formation_choice == 10 then
        enemies = generate_diamond_formation(center_x, center_y, adjusted_enemies_per_wave, spacing)
    end

    -- Générer les ennemis avec la formation choisie
    local spawned_enemies = {}
    for _, enemy in ipairs(enemies) do
        table.insert(spawned_enemies, {
            type = enemy.type,
            script = enemy.script,
            x = enemy.x,
            y = enemy.y,
            collision_x = enemy.collision_x,
            collision_y = enemy.collision_y,
            health = enemy.health
        })
        active_enemies_count = active_enemies_count + 1
    end

    --Afficher des informations de debug
    print("Wave generated with " .. #spawned_enemies .. " enemies")
    print("Total active enemies: " .. active_enemies_count)

    return {
        enemies = spawned_enemies
    }
end

SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

local function generate_boss(current_time)
    -- Chercher le boss avec le tag correspondant au niveau
    for index, boss in ipairs(boss_types) do
        if boss.tag ==  level then
            -- Créer le boss
            local boss_instance = {
                type = boss.type,
                script = boss.script,
                collision_x = boss.collision_x,
                collision_y = boss.collision_y,
                x = boss.posX,
                y = boss.posY,
                health = boss.health,
                nb_type = boss.nb_type,
                tag = boss.tag
            }

            -- Indiquer qu'un boss est actif
            boss_active = true
            last_boss_time = current_time

            print("Generated boss: " .. boss.type .. " (Tag: " .. boss.tag .. ")")

            return boss_instance
        end
    end

    -- Si aucun boss n'est trouvé avec le tag
    print("Aucun boss disponible pour le niveau avec le tag : " .. level)
    return nil
end

-- Définir les paramètres de difficulté pour chaque niveau
local DIFFICULTY_SETTINGS = {
    ["Blue_Nebular"] = {
        wave_interval = 2,
        wave_cooldown = 3,
        max_waves = 3,
        boss_wave_delay = 5 -- Nombre de vagues avant le boss
    },
    ["Green_Nebular"] = {
        wave_interval = 3,
        wave_cooldown = 2,
        max_waves = 10,
        boss_wave_delay = 6
    },
    ["Purple_Nebular"] = {
        wave_interval = 2,
        wave_cooldown = 3,
        max_waves = 20,
        boss_wave_delay = 7
    },
    ["Red_Nebular"] = {
        wave_interval = 2,
        wave_cooldown = 2,
        max_waves = 30,
        boss_wave_delay = 8
    },
    ["Yellow_Nebular"] = {
        wave_interval = 1.5,
        wave_cooldown = 2,
        max_waves = 42,
        boss_wave_delay = 9
    },
    ["EndLevel"] = {
        wave_interval = 1,
        wave_cooldown = 1.5,
        max_waves = -1, -- Infini
        boss_wave_delay = 10
    }
}

-- Fonction pour obtenir les paramètres de difficulté actuels
local function get_difficulty_params(level_name)
    return DIFFICULTY_SETTINGS[level_name] or DIFFICULTY_SETTINGS["Blue_Nebular"]
end

-- Fonction à appeler lorsqu'un ennemi est détruit
function on_enemy_destroyed()
    if active_enemies_count > 0 then
        active_enemies_count = active_enemies_count - 1
        --print("Enemy destroyed. Remaining active enemies: " .. active_enemies_count)
    end
end

local last_wave_time = 0
local boss_active = false
local boss_cooldown = 30           -- Temps entre deux boss
level  = ""

local last_boss_wave = 0
function set_boss_inactive()
    boss_active = false
    print("Boss marqué comme inactif.")
end


-- Fonction pour mettre à jour le niveau
function update_level(current_time)
    local difficulty = get_difficulty_params(level)
    local time_since_last_wave = current_time - last_wave_time
    local time_since_last_boss = current_time - last_boss_time

    -- Ne pas générer de vagues ou de boss si un boss est actif
    if boss_active then
        --print("Boss actif, aucune vague ou nouveau boss ne sera généré.")
        return nil
    end

    -- Gestion des vagues normales
    if time_since_last_wave >= difficulty.wave_interval and active_enemies_count < MAX_ACTIVE_ENEMIES then
        last_wave_time = current_time
        current_wave = current_wave + 1
        local wave_result = generate_wave()

        print("Vague " .. current_wave .. " générée.")

        -- Vérifier si nous devons préparer un boss après 20 vagues
        if current_wave >= last_boss_wave + 30 then
            print("Prochain boss sera généré dans la prochaine opportunité.")
            return {warning = "Boss incoming in 5 seconds!"}
        end

        return wave_result
    end

    -- Gestion des moments épiques pour les boss
    if current_wave >= last_boss_wave + 40 then
        local boss_spawn_time = last_boss_time + difficulty.boss_wave_delay * difficulty.wave_interval

        -- Pré-avertir 5 secondes avant le spawn du boss
        -- if current_time >= (boss_spawn_time - 5) and current_time < boss_spawn_time then
        --     print("Avertissement : Le boss arrive dans 5 secondes !")
        --     return {warning = "Boss incoming in 5 seconds!"}
        -- end

        -- Générer le boss si le temps est venu
        if current_time >= boss_spawn_time then
            print("Epic moment: Génération du boss pour le niveau " .. level)
            local boss = generate_boss(current_time)
            boss_active = true
            last_boss_time = current_time
            last_boss_wave = current_wave
            return {boss = boss}
        end
    end

    return nil
end

function set_level(_level)
    level = _level
end
