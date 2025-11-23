/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** network_update
*/

#include "../include/network.hpp"

std::vector<std::pair<std::string, int>> getSortedScoreboard(std::map<int, std::string>& players_uuids, std::unordered_map<Entity, int>& players_scores)
{
    // Vector pour contenir les scores
    std::vector<std::pair<std::string, int>> scoreboard;

    // Associer les noms aux scores
    for (const auto& [entity, score] : players_scores) {
        auto it = players_uuids.find(entity);

        if (it != players_uuids.end()) {
            scoreboard.emplace_back(it->second, score);
        }
    }

    // Trier le vecteur par ordre décroissant des scores
    std::sort(scoreboard.begin(), scoreboard.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Tri décroissant par score
    });

    return scoreboard;
}

void GameServer::Handle_game_end()
{
    // Obtenir le scoreboard trié initialement
    std::vector<std::pair<std::string, int>> scoreboard = getSortedScoreboard(_players_uuids, collision_system->getPlayersScore());

    // Utiliser un std::unordered_map pour éliminer les doublons et garder les meilleurs scores
    std::unordered_map<std::string, int> uniqueScores;
    for (const auto& [name, score] : scoreboard) {
        if (uniqueScores.find(name) != uniqueScores.end()) {
            uniqueScores[name] = std::max(uniqueScores[name], score); // Garder le score le plus élevé
        } else {
            uniqueScores[name] = score;
        }
    }

    // Reconstruire le scoreboard sans doublons
    scoreboard.clear();
    for (const auto& entry : uniqueScores) {
        scoreboard.emplace_back(entry.first, entry.second);
    }

    // Trier le scoreboard par score décroissant
    std::sort(scoreboard.begin(), scoreboard.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Tri par score décroissant
    });

    // Affichage du scoreboard trié dans la console (pour débogage)
    std::cout << "Scoreboard (après suppression des doublons et tri) :\n";
    for (const auto& [name, score] : scoreboard) {
        std::cout << name << ": " << score << std::endl;
    }

    // Convertir le scoreboard en un message binaire
    std::vector<char> scorebord_msg = BinaryProtocol::GameFinish(scoreboard);

    // Envoyer le message à tous les clients
    Send_to_all_client(scorebord_msg);

    // Indiquer que le jeu est terminé
    game_finish = 1;
}

void GameServer::handle_boss_death()
{
    for (size_t i = 0; i < _levels.size(); ++i) {
        std::cout << "Level " << i << ": " << _levels[i].getLevelName() << std::endl;
    }
    
    std::vector<char> msg_score = BinaryProtocol::GetScore(_score);
    Send_to_all_client(msg_score);
    levelScript.executeFunction("set_boss_inactive");
    std::vector<char> msg_death = BinaryProtocol::GetBossDeath();
    Send_to_all_client(msg_death);

    if (!in_transition && current_level_num + 1 < _levels.size()) {
        current_level_num++;
        current_level = _levels[current_level_num].getLevelName();
        levelScript.executeFunction("set_level", current_level);

        //std::cout << "Before level " << 
        collision_system->AdeadEnemy(100);
        //_score += 1000;
        _score = collision_system->getdeadEnemy() * _score_multipliyer;
        std::cout << "Score is :" << _score;
        
        // reset players life

        for (const auto& client_info : _clients) {
            int player_id = client_info.getID();
            auto &health = _mediator.GetComponent<Health>(_logic.getEntity(player_id));
            health.life = _players_init_health;
        }

        std::vector<char> next_level_msg = BinaryProtocol::LevelInfo(current_level, current_level_num, _levels.size());
        Send_to_all_client(next_level_msg);
    }

    in_transition = true;
    boss_system->SetBoosState(false);

    std::vector<char> msg_level = BinaryProtocol::GetLevel(current_level);
    Send_to_all_client(msg_level);
}

void GameServer::Physic_update(float deltaTime)
{

    auto now = std::chrono::steady_clock::now();
    level_time_elapsed = std::chrono::duration<float>(now - level_start_time).count();
    
    // if (current_level == "End_level") {
    //     game_finish = 1;
    //     Handle_game_end();
    // }

    // if (game_finish == 1) {
    //     Handle_game_end();
    // }
    if (!in_transition && game_finish == 0) {
        updateLevel(level_time_elapsed);
    }
    
    time_since_last_spawn += deltaTime;
    
    if (time_since_last_spawn >= 2.0f) {
        spawn_power_ups();
        time_since_last_spawn = 0.0f; // Réinitialise le temps depuis le dernier spawn
    }

        // Mises à jour des entités
        _movement_system->Movement(_mediator);
        projectile_system->Movement(_mediator);
        _obstacle_system->Update(_mediator);

        // Détecter les collisions et récupérer les entités à supprimer
        _enemyToDelete = enemy_movement_system->Movement(_mediator, deltaTime);
        enemy_movement_system->Update_bullet(_mediator, deltaTime);
        _entityToDelete = collision_system->updateCollision(_mediator);
        _entityToDelete = projectile_system->check_out(_mediator, _entityToDelete);
        collectable_system->Update(_mediator, _entityToDelete);
        _entityToDelete = enemyBullet_system->Update_bullet(_mediator, level_time_elapsed, _entityToDelete);
        _score = collision_system->getdeadEnemy() * _score_multipliyer;
        _entityToDelete = boss_system->Update_Boss(_mediator, _entityToDelete, level_time_elapsed);

        // Vérifier si le boss est tué
        if (boss_system->GetBossState() == true) {
            handle_boss_death();
            transition_start_time = now;
            //return;
        }

        // Supprimer les entités marquées pour la suppression
        if (!_enemyToDelete.empty()) {
            for (const Entity& ent : _enemyToDelete) {
                levelScript.executeFunction("on_enemy_destroyed");
            }
        }

        if (!_entityToDelete.empty()) {
            for (const Entity& ent : _entityToDelete) {
                // Recherche et suppression de l'entité dans la liste des ennemis
                auto itEnemy = std::find(_logic.GetEnemyEntityList().begin(), _logic.GetEnemyEntityList().end(), ent);
                if (itEnemy != _logic.GetEnemyEntityList().end()) {
                    levelScript.executeFunction("on_enemy_destroyed");
                }
            }
        }
    //}

    // Transition entre niveaux (délai après la mort du boss)
    if (in_transition && game_finish == 0) {
        float transition_time_elapsed = std::chrono::duration<float>(now - transition_start_time).count();
        float time_remaining = 15.0f - transition_time_elapsed;


        std::vector<char> msg_time = BinaryProtocol::GetTimer(time_remaining);
        Send_to_all_client(msg_time);
    
        // Envoyer le temps restant avant le prochain niveau
        //send_time_to_next_level(time_remaining);

        if (transition_time_elapsed >= 15.0f) {
            std::vector<char> msg_text = BinaryProtocol::NextLevel();
            Send_to_all_client(msg_text);
            in_transition = false;
        }
    }

    if (current_level == "EndLevel") {
        Handle_game_end();
    }
}

void GameServer::Current_game_update()
{

}

void GameServer::fixedUpdate(float deltaTime)
{
    Physic_update(deltaTime);
}

void GameServer::network_update()
{
    if (game_staterd == 1) {
        std::vector<char> msg = BinaryProtocol::GetLevel(current_level);
        Send_to_all_client(msg);
        game_staterd = 0;
    }
    Game_update();
    Boss_update();
    Update_player();
    Update_collsion();
    Update_projectile();
    Update_enemy_bullets();
    Update_enemy();
    Update_power_up();
    Update_deleted_entities();
}

void GameServer::Game_update()
{
    std::vector<char> msg = BinaryProtocol::GetScore(_score);
    Send_to_all_client(msg);

    std::vector<char> level_time = BinaryProtocol::GetLevelTime(level_time_elapsed);
    Send_to_all_client(level_time);
}

void GameServer::Boss_update()
{
    if (!boss_system->getBullets().empty()) {
        for (const Bullet & bullet : boss_system->getBullets()) {

            Entity enemy_bullet = _mediator.createEntity();
            _logic.AddEnemyBullet(enemy_bullet, bullet.x, bullet.y, bullet.damage, bullet.type, bullet.script, bullet.colx, bullet.coly);
            enemyBullet_system->init_bullet(enemy_bullet, _mediator, bullet.x, bullet.y, bullet.angle, bullet.speed);

            std::cout << "Logic " << "Spawned bullet " << enemy_bullet << " at " << bullet.x << " " << bullet.y << " with type " << bullet.type << std::endl;
        
            std::vector<char> msg = BinaryProtocol::GetBossBulletCreate(enemy_bullet, bullet.x, bullet.y, bullet.type, 1);

            Send_to_all_client(msg);
            //EnemyBulletToCreate.push_back(enemy_bullet);   
        }
        boss_system->getBullets().clear();
    }
    if (!BossToCreate.empty()) {
        for (const auto& boss : BossToCreate) {

            auto &position = _mediator.GetComponent<Position>(boss.second);
            auto &health = _mediator.GetComponent<Health>(boss.second);

            int x = position.x;
            int y = position.y;
            int boss_health = health.life;
            std::string boss_name = boss.first;

            std::vector<char> msg = BinaryProtocol::GetBossCreate(boss.second, x, y, 1, boss_name, boss_health);

            Send_to_all_client(msg);
        }
        BossToCreate.clear();
    } 

    // update the player
    if (!_logic.getBossList().empty()) {
        for (const auto& boss : _logic.getBossList()) {
            auto &position = _mediator.GetComponent<Position>(boss);
            auto &health = _mediator.GetComponent<Health>(boss);

            int x = position.x;
            int y = position.y;
            int boss_health = health.life;
            std::vector<char> msg = BinaryProtocol::GetBossUpdate(boss, x, y, 1, boss_health);

            //std::cout << "Boss update Positon x = " << x << " y = " << y << "\n";
            Send_to_all_client(msg);
        }
        // send the 
    }
}

void GameServer::Generate_obstacles()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const float spriteWidth = 100.0f;
    const float spriteHeight = 100.0f;

    std::vector<int> top_sprite_ids;
    std::vector<int> top_sprite_x;
    std::vector<int> top_sprite_y;

    std::vector<int> back_sprite_ids;
    std::vector<int> back_sprite_x;
    std::vector<int> back_sprite_y;
    std::vector<int> types;

    std::vector<int> back_types;

    int spritesNeeded = static_cast<int>(std::ceil(screenWidth / spriteWidth)) * 2;

    std::cout << "sprite needed ==" << spritesNeeded << "\n";
    // get all the obastacles position
    for (int i = 0; i < spritesNeeded; ++i) {
        //top_sprite_ids.push_back(i);

        //std::cout << "Generate top at position = " << i * spriteWidth << "\n";
        top_sprite_x.push_back(i * spriteWidth);
        top_sprite_y.push_back(0);
        //types.push_back(1);

        //back_sprite_ids.push_back(i);
        back_sprite_x.push_back(i * spriteWidth);
        back_sprite_y.push_back(screenHeight - spriteHeight - 10);
        //std::cout << "Generate top at position x = " << i * spriteWidth << "y = " << screenHeight - spriteHeight << "\n";
        //back_types.push_back(2);
    }
    // create all the obstacles entity
    for (int i = 0; i < spritesNeeded / 2; i++) {
        Entity obstacle = _mediator.createEntity();
        _logic.AddObstacle(obstacle, top_sprite_x[i], top_sprite_y[i], 4);
        top_sprite_ids.push_back(obstacle);
        //_mediator.createEntity("obstacle", top_sprite_ids[i], top_sprite_x[i], top_sprite_y[i]);
    }
    std::vector<char> msg = BinaryProtocol::GetObstacleCreate(top_sprite_ids.data(), top_sprite_ids.size(), top_sprite_x.data(), top_sprite_y.data(), 1);
    Send_to_all_client(msg);

    for (int i = spritesNeeded / 2; i < spritesNeeded; i++) {
        Entity obstacle = _mediator.createEntity();
        _logic.AddObstacle(obstacle, back_sprite_x[i], back_sprite_y[i], 4);
        back_sprite_ids.push_back(obstacle);
        //_mediator.createEntity("obstacle", top_sprite_ids[i], top_sprite_x[i], top_sprite_y[i]);
    }

    std::vector<char> msg_back = BinaryProtocol::GetObstacleCreate(back_sprite_ids.data(), back_sprite_ids.size(), back_sprite_x.data(), back_sprite_y.data(), 2);
    Send_to_all_client(msg_back);
    game_staterd = 0;
}

void GameServer::start_update()
{
    const float fixed_timestep = 0.01667f; // 60 Hz
    const float network_update_interval = 0.05f; // 20 Hz

    static float physics_accumulator = 0.0f;
    static float network_accumulator = 0.0f;

    auto previous_time = std::chrono::steady_clock::now();

    _timer.expires_after(std::chrono::milliseconds(1));
    _timer.async_wait([this, fixed_timestep, network_update_interval, previous_time]
    (const boost::system::error_code& error) mutable {

        if (!error) {
            auto current_time = std::chrono::steady_clock::now();
            float delta_time = std::chrono::duration<float>(current_time - previous_time).count();
            previous_time = current_time;

            physics_accumulator += delta_time;
            network_accumulator += delta_time;

            // Mettre à jour la physique
            while (physics_accumulator >= fixed_timestep) {
                fixedUpdate(delta_time);
                //update_physics(fixed_timestep);
                physics_accumulator -= fixed_timestep;
            }

            // Envoyer des mises à jour aux clients
            if (network_accumulator >= network_update_interval) {
                network_update();
                //send_updates_to_clients();
                network_accumulator -= network_update_interval;
            }
            // Relancer la boucle
            start_update();
        }
    });
}

void GameServer::Update_player()
{
    std::vector<int> player_ids{};
    std::vector<int> player_posX{};
    std::vector<int> player_posY{};
    std::vector<int> player_health{};

    // Collect player positions
    for (const auto& client_info : _clients) {
        int player_id = client_info.getID();
        player_ids.push_back(player_id);
        auto& position = _mediator.GetComponent<Position>(_logic.getEntity(player_id));
        auto &health = _mediator.GetComponent<Health>(_logic.getEntity(player_id));

       //std::cout << "Player update data x = " << position.x << " y = " << position.y << "\n";
       player_posX.push_back(position.x);
       player_posY.push_back(position.y);
       player_health.push_back(health.life);
    }

    // Send updates to clients
    std::vector<char> player_update_data = BinaryProtocol::GetClientUpdateAll(
        player_ids.data(), player_ids.size(), player_posX.data(), player_posY.data()
    );
    Send_to_all_client(player_update_data);

    // send all players life
    std::vector<char> players_life = BinaryProtocol::GetPlayerHealth(
        player_ids.data(), player_health.data(), player_ids.size()
    );
    Send_to_all_client(players_life);

}

void GameServer::Update_enemy()
{
    //std::cout << "ENNNT ====================      " << entitiesToCreate.size() << "\n";
    if (!entitiesToCreate.empty()) {
        for (auto & entity : entitiesToCreate) {
            auto & position = _mediator.GetComponent<Position>(entity.second);
            auto & sc = _mediator.GetComponent<Script>(entity.second);
            int x = position.x;
            int y = position.y;
            //std::cout << "Server enemy " << entity.second << " pos.x " << x << " pos.y == " << y << "\n";
            std::vector<char> msg = BinaryProtocol::GetEnemyCreate(entity.second, 1, x, y, entity.first);
            Send_to_all_client(msg);
            //sendEntityToClients(entity);
        }
        entitiesToCreate.clear();
    }
    
    if (!_logic.GetEnemyEntityList().empty()) {
        std::vector<int> enemy_ids{};
        std::vector<int> enemy_posX{};
        std::vector<int> enemy_posY{};

        // collect enemy position
        for (int i = 0; i < _logic.GetEnemyEntityList().size(); i++) {
            enemy_ids.push_back(_logic.GetEnemyEntityList()[i]);
            auto& position = _mediator.GetComponent<Position>(_logic.GetEnemyEntityList()[i]);

            //std::cout << " Update Enemy " << _logic.GetEnemyEntityList()[i] << "position x" << position.x << " position y " << position.y << std::endl;
            enemy_posX.push_back(position.x);
            enemy_posY.push_back(position.y);
        }

        std::vector<char> enemy_update_data = BinaryProtocol::GetEnemyListUpdate(
            enemy_ids.data(), enemy_ids.size(), enemy_posX.data(), enemy_posY.data()
        );
        Send_to_all_client(enemy_update_data);
    }
}

void GameServer::Update_power_up()
{
    if (!PowerUpToCreate.empty()) {
        for (auto & entity : PowerUpToCreate) {
            auto & position = _mediator.GetComponent<Position>(entity.second);
            std::string name = entity.first;
            int x = position.x;
            int y = position.y;

            std::vector<char> msg = BinaryProtocol::GetPowerCreate(entity.second, x, y, name);
            //std::cout << "Server enemy " << entity.second << " pos.x " << x << " pos.y == " << y << "\n";
            Send_to_all_client(msg);
        }
        PowerUpToCreate.clear();
    }

    if (!_logic.getpowerList().empty()) {
        for (auto & entity : _logic.getpowerList()) {
            auto & position = _mediator.GetComponent<Position>(entity);

            //std::cout << "Server spwan power " << entity << " pos.x " << position.x << " pos.y == " << position.y << "\n";
            std::vector<char> msg = BinaryProtocol::GetPowerUpdate(entity, position.x, position.y);

            Send_to_all_client(msg);
        }
    }
}

void GameServer::Update_projectile()
{
    if (!_logic.GetProjectileList().empty()) {
        int *shoot_ids = _logic.GetProjectileList().data();
        std::vector<int> shoot_posX{};
        std::vector<int> shoot_posY{};
        std::vector<int> types{};

        // collect the informations of the bullet
        for (int i = 0; i < _logic.GetProjectileList().size(); i++) {
            auto& bullet_position = _mediator.GetComponent<Position>(_logic.GetProjectileList()[i]);
            //auto &shoot = _mediator.GetComponent<Position>(_logic.GetProjectileList()[i]);
            //std::cout << "Update position of bullet " << bullet_id << "pos x == " << bullet_position.x << "pos y == " << bullet_position.y << "\n";
            shoot_posX.push_back(bullet_position.x);
            shoot_posY.push_back(bullet_position.y);
            types.push_back(0);
        }

        // Send the updated positions of all bullets to clients
        std::vector<char> projectile_update_data = BinaryProtocol::GetprojectileListUpdate(
            shoot_ids, shoot_posX.size(), shoot_posX.data(), shoot_posY.data(), types.data()
        );
        Send_to_all_client(projectile_update_data);
    }
}

void GameServer::Update_deleted_entities()
{
    // Supprimer les entités
    if (!_enemyToDelete.empty()) {
        for (const Entity ent : _enemyToDelete) {
            _mediator.deleteEntity(ent);

            auto it = std::find(_logic.GetEnemyEntityList().begin(), _logic.GetEnemyEntityList().end(), ent);
            _logic.GetEnemyEntityList().erase(it);

            std::vector<char> msg = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msg);
        }
    } else if (!_entityToDelete.empty()) {
        Delete_ent_simple(_entityToDelete);
    }
}

void GameServer::Update_collsion()
{
    if (!collision_system->getCollidedEnity().empty()) {
        std::vector<int> types{};
        std::vector<int> x{};
        std::vector<int> y{};

        for (const Collider ent_col : collision_system->getCollidedEnity()) {
            types.push_back(ent_col.type);
            x.push_back(ent_col.x);
            y.push_back(ent_col.y);
        }

        // send to the client all ocuured collsion
        std::vector<char> msg = BinaryProtocol::GetAllColllision(types.data(), x.data(), y.data(), collision_system->getCollidedEnity().size());
        Send_to_all_client(msg);

        collision_system->getCollidedEnity().clear();
    }
}

void GameServer::Update_enemy_bullets()
{
    if (!enemy_movement_system->getEnemyBullet().empty()) {
        for (const Bullet & bullet : enemy_movement_system->getEnemyBullet()) {
            Entity enemy_bullet = _mediator.createEntity();
            std::vector<int> x{0};
            std::vector<int> y{0};
            std::vector<int> types{0};
            std::vector<int> ids{0};

            _logic.AddEnemyBullet(enemy_bullet, bullet.x, bullet.y, bullet.damage, bullet.type, bullet.script, bullet.colx, bullet.coly);
            enemyBullet_system->init_bullet(enemy_bullet, _mediator, bullet.x, bullet.y, bullet.angle, bullet.speed);
    

            std::vector<char> msg = BinaryProtocol::GetEnemyBulletCreate(enemy_bullet, bullet.x, bullet.y, bullet.type, 1);
            //std::cout << "Created enemy  bullet entity: " << enemy_bullet << " at position (" << bullet.x << ", " << bullet.y << ")\n";
            Send_to_all_client(msg);

        }
        enemy_movement_system->getEnemyBullet().clear();
    }

    if (!_logic.GetEnemyBullet().empty()) {
        std::vector<int> x{};
        std::vector<int> y{};
        std::vector<int> types{};
        std::vector<int> ids{};

        // get all the enemy bullet informations
        for (const Entity &ent : _logic.GetEnemyBullet()) {
            auto &positon = _mediator.GetComponent<Position>(ent);
            auto &type = _mediator.GetComponent<Bullet_type>(ent);
            ids.push_back(ent);
            x.push_back(positon.x);
            y.push_back(positon.y);
            types.push_back(type.type);


            //std::cout << "Enemy Bullet update at " << positon.x << " " << positon.y << std::endl;
            // send the message of the creation to all the client
        }
        std::vector<char> msg = BinaryProtocol::GetEnemyBulletUpdate(ids.data(),x.data(), y.data(), _logic.GetEnemyBullet().size());

        //send the new enemy bullets to the clients
        Send_to_all_client(msg);
    }
}
