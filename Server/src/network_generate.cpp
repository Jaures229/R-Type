/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** network_generate
*/

#include "../include/network.hpp"


void GameServer::GenerateProceduralLevel()
{
    
}

void GameServer::updateLevel(float currentTime)
{
    auto result = levelScript.executeFunction("update_level", currentTime);

    if (result.valid() && result.get_type() == sol::type::table) {
        sol::table events = result;

        // Gérer les avertissements de boss
        if (events["warning"].valid() && events["warning"].get_type() == sol::type::string) {
            std::string warningMessage = events["warning"];
            std::cout << "Boss Warning: " << warningMessage << std::endl;
            //is_warning = 1;

            std::cout << "Boss warning is " << is_warning << "\n";

            // send to the client the warning that a boss is comming
            //for (const auto& boss : _logic.getBossList()) {
            std::vector<char> msg_warning = BinaryProtocol::GetBossIncomming(0, 0, 0);
            std::cout << "Sended incomming boss msg\n";
            Send_to_all_client(msg_warning);
            //}
            //is_warning = 0;
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


void GameServer::spawnEnemy(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health)
{
    Entity enemy = _mediator.createEntity();
    Entity compy = enemy;
    _logic.AddNewEnemy(enemy, type, script, x, y, colx, coly, health);
    //std::cout << "Spawned enemy: " << enemy << " at (" << x << ", " << y << ")" << std::endl;

    auto &enemy_script = _mediator.GetComponent<Script>(enemy);
    enemy_script.executeFunction("init_enemy", x, y);


    std::pair<std::string, Entity> temp_value; 
    temp_value.first = type;
    temp_value.second = compy;
    entitiesToCreate.push_back(temp_value);
}

void GameServer::spawnBoss(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health, int nb_type)
{
    Entity Boss = _mediator.createEntity();
    _logic.AddBoss(Boss, x, y, type, health, colx, coly, nb_type, script);
    std::cout << "Spawned boss: " << Boss << " " << type <<  " " << " at (" << x << ", " << y << ") " << "Health" << health << "\n";

    boss_system->Init_Boss(Boss, _mediator, x, y, health);
    BossToCreate.push_back({type, Boss});
}

std::string GameServer::Generate_UUID(void)
{
    boost::uuids::uuid uuid = generator();

    const std::string random_uuid = boost::uuids::to_string(uuid);
    //boost::lexical_cast<std::string>(generator());
    return random_uuid;
}

void GameServer::Delete_ent_enemy(std::unordered_set<Entity> enemyToDelete)
{
}

void GameServer::Delete_ent_simple( std::unordered_set<Entity> entityToDelete)
{
    for (const Entity& ent : entityToDelete) {
        // Recherche et suppression de l'entité dans la liste des ennemis
        auto itEnemy = std::find(_logic.GetEnemyEntityList().begin(), _logic.GetEnemyEntityList().end(), ent);
        if (itEnemy != _logic.GetEnemyEntityList().end()) {
            _mediator.deleteEntity(ent);
            _logic.GetEnemyEntityList().erase(itEnemy);

            // Génération et envoi d'un message de suppression pour les clients (si applicable)
            std::vector<char> msgEnemy = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msgEnemy);
        }

        // Recherche et suppression de l'entité dans la liste des projectiles
        auto itProjectile = std::find(_logic.GetProjectileList().begin(), _logic.GetProjectileList().end(), ent);
        if (itProjectile != _logic.GetProjectileList().end()) {
            _mediator.deleteEntity(ent);
            _logic.GetProjectileList().erase(itProjectile);
            // Génération et envoi d'un message de suppression pour les clients (si applicable)
            std::vector<char> msgProjectile = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msgProjectile);
        }
        // Recherche et suppression de l'entité dans la liste des powerups
        auto itPowerUp = std::find(_logic.getpowerList().begin(), _logic.getpowerList().end(), ent);
        if (itPowerUp != _logic.getpowerList().end()) {
            _mediator.deleteEntity(ent);
            _logic.getpowerList().erase(itPowerUp);
            // Génération et envoi d'un message de suppression pour les clients (si applicable)
            std::vector<char> msgEnemy = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msgEnemy);
        }

        auto itEnemyBullet = std::find(_logic.GetEnemyBullet().begin(), _logic.GetEnemyBullet().end(), ent);
        if (itEnemyBullet != _logic.GetEnemyBullet().end()) {
            _mediator.deleteEntity(ent);
            _logic.GetEnemyBullet().erase(itEnemyBullet);

            // Génération et envoi d'un message de suppression pour les clients (si applicable)
            std::vector<char> msgEnemyBullet = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msgEnemyBullet);
        }


        auto itBoss = std::find(_logic.getBossList().begin(), _logic.getBossList().end(), ent);
        if (itBoss != _logic.getBossList().end()) {
            _mediator.deleteEntity(ent);
            _logic.getBossList().erase(itBoss);

            // Génération et envoi d'un message de suppression pour les clients (si applicable)
            std::vector<char> msgEnemyBullet = BinaryProtocol::GetEnemyDelete(ent);
            Send_to_all_client(msgEnemyBullet);
        }
    }
}

std::optional<PowerUp> spawnPowerUp(const std::vector<std::string>& powerUps, int screenWidth, int screenHeight)
{
    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> spawnChance(0.0, 1.0);
    std::uniform_int_distribution<> powerUpIndex(0, powerUps.size() - 1);

    // Chance de spawn 50%
    if (spawnChance(gen) > 0.9) {
        return std::nullopt; // Aucun power-up n'apparaît
    }

    // Sélectionne un power-up aléatoire
    std::string selectedPowerUp = powerUps[powerUpIndex(gen)];

    // Positionne le power-up aléatoirement près du centre de l'écran
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    std::uniform_int_distribution<> offsetX(-100, 100); // Plage d'écart en X
    std::uniform_int_distribution<> offsetY(-100, 100); // Plage d'écart en Y

    int spawnX = centerX + 100;
    int spawnY = centerY + 100;
    // Retourne le power-up avec ses coordonnées
    return PowerUp{selectedPowerUp, centerX, centerY};
}

void GameServer::spawn_power_ups()
{
    int screenWidth = 1900;
    int screenHeight = 800;

    // Essai de génération de power-ups
    auto spawnedPowerUp = spawnPowerUp(powerUps, screenWidth, screenHeight);

    if (spawnedPowerUp) {
        Entity new_power_up = _mediator.createEntity();
        //Entity compy = new_power_up;

        if (spawnedPowerUp->name == "Speed") {
            _logic.AddPowerUp(new_power_up, spawnedPowerUp->x, spawnedPowerUp->y, spawnedPowerUp->name, 64, 32, 0, power_speed, 0);
        } else if (spawnedPowerUp->name == "Health") {
            _logic.AddPowerUp(new_power_up, spawnedPowerUp->x, spawnedPowerUp->y, spawnedPowerUp->name, 64, 32, power_health, 0, 0);
        } else if (spawnedPowerUp->name == "Damage") {
            _logic.AddPowerUp(new_power_up, spawnedPowerUp->x, spawnedPowerUp->y, spawnedPowerUp->name, 64, 32, 0, 0, power_damage);
        }
        std::pair<std::string, Entity> powerUpPair(spawnedPowerUp->name, new_power_up);

        PowerUpToCreate.push_back(powerUpPair);
        // std::cout << "Power-up spawned: " << spawnedPowerUp->name
        //           << " at position (" << spawnedPowerUp->x << ", " << spawnedPowerUp->y << ")\n";
       // _logic.getpowerList().push_back(new_power_up);
    } else {
        //std::cout << "No power-up spawned this time.\n";
    }
}
