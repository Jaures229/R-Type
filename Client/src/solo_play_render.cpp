/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** solo_play_render
*/

#include "../include/SoloPlay.hpp"

void SoloPlay::render(float deltaTime)
{
    _windows.draw(chargeBarBackground);
    _windows.draw(chargeBar);
    _windows.draw(_manager.getBackgroundSprites()[current_level].sprite);

    _render_system->Render(_windows, _mediator);
    _animation_system->update(_mediator, deltaTime);
;
}

void SoloPlay::run()
{
    fixedUpdate();
}

void SoloPlay::fixedUpdate()
{
    float delta_time = clock.restart().asSeconds();

    physics_accumulator += delta_time;

    while (physics_accumulator >= physics_time_step) {
        updatePhysics(physics_time_step);
        physics_accumulator -= physics_time_step;
    }
    //std::cout << "Del"
    render(delta_time);
}

void SoloPlay::updatePhysics(float deltaTime)
{
    auto now = std::chrono::steady_clock::now();
    level_time_elapsed = std::chrono::duration<float>(now - level_start_time).count();

    // Mises à jour des entités
    _movement_system->Movement(_mediator);
    projectile_system->Movement(_mediator);
    _obstacle_system->Update(_mediator);

    update_level(level_time_elapsed);

    _enemyToDelete = enemy_movement_system->Movement(_mediator, deltaTime);
    enemy_movement_system->Update_bullet(_mediator, deltaTime);
    _entityToDelete = collision_system->updateCollision(_mediator);
    _entityToDelete = projectile_system->check_out(_mediator, _entityToDelete);
    collectable_system->Update(_mediator, _entityToDelete);
    _entityToDelete = enemyBullet_system->Update_bullet(_mediator, level_time_elapsed, _entityToDelete);
    _score = collision_system->getdeadEnemy() * 10;
    _entityToDelete = boss_system->Update_Boss(_mediator, _entityToDelete, level_time_elapsed);


    if (!_enemyToDelete.empty()) {
        for (const Entity ent : _enemyToDelete) {
            _mediator.deleteEntity(ent);

        }
    }

    if (!_entityToDelete.empty()) {
        for (const Entity ent : _entityToDelete) {
            _mediator.deleteEntity(ent);
        }
    }
}

void SoloPlay::update_level(float currentTime)
{
    auto result = levelScript.executeFunction("update_level", currentTime);

    if (result.valid() && result.get_type() == sol::type::table) {
        sol::table events = result;

        // Gérer les avertissements de boss
        if (events["warning"].valid() && events["warning"].get_type() == sol::type::string) {
            std::string warningMessage = events["warning"];
            std::cout << "Boss Warning: " << warningMessage << std::endl;
            is_warning = 1;

            std::cout << "Boss warning is " << is_warning << "\n";
        }

        // Générer des ennemis
        if (events["enemies"].valid() && events["enemies"].get_type() == sol::type::table) {
            sol::table enemies = events["enemies"];
            for (auto& pair : enemies) {
                sol::table enemy = pair.second.as<sol::table>();
                spawnEnemy(
                    enemy["type"].get<std::string>(),
                    enemy["script"].get<std::string>(),
                    enemy["x"].get<float>(),
                    enemy["y"].get<float>(),
                    enemy["collision_x"].get<int>(),
                    enemy["collision_y"].get<int>(),
                    enemy["health"].get<int>()
                );
            }
        }

        // Générer un boss
        if (events["boss"].valid() && events["boss"].get_type() == sol::type::table) {
            std::cout << "Spawning boss..." << std::endl;
            sol::table boss = events["boss"];
            spawnBoss(
                boss["type"].get<std::string>(),
                boss["script"].get<std::string>(),
                boss["x"].get<float>(),
                boss["y"].get<float>(),
                boss["collision_x"].get<int>(),
                boss["collision_y"].get<int>(),
                boss["health"].get<int>(),
                boss["nb_type"].get<int>()
            );
        }
    }
}

void SoloPlay::spawnEnemy(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health)
{
    Entity enemy = _mediator.createEntity();
    Entity compy = enemy;
    _logic.AddNewEnemy(enemy, type, script, x, y, colx, coly, health);

    auto &enemy_script = _mediator.GetComponent<Script>(enemy);
    enemy_script.executeFunction("init_enemy", x, y);

    _mediator.AddComponent(enemy, _manager.getSpriteMap()[type]);
    _mediator.AddComponent(enemy, _manager.getAnimationMap()[type]);
    _animation_system->setAnimation(_mediator, "idle", enemy);
}

void SoloPlay::spawnBoss(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health, int nb_type)
{
    Entity Boss = _mediator.createEntity();
    _logic.AddBoss(Boss, x, y, type, health, colx, coly, nb_type, script);
    std::cout << "Spawned boss: " << Boss << " " << type <<  " " << " at (" << x << ", " << y << ") " << "Health" << health << "\n";

    boss_system->Init_Boss(Boss, _mediator, x, y, health);
    _mediator.AddComponent(Boss, _manager.getSpriteMap()[type]);
    _mediator.AddComponent(Boss, _manager.getAnimationMap()[type]);
    _animation_system->setAnimation(_mediator, "idle", Boss);    
}
