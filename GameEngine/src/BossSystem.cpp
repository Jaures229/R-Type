/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** BossSystem
*/

#include "../Include/BossSystem.hpp"

void BossSystem::Init_Boss(Entity boss, Mediator & _medi, int posx, int posy, int health)
{
    auto &script = _medi.GetComponent<Script>(boss);
    script.executeFunction("init_boss", posx, posy, health);
}

void BossSystem::Update_Boss_bullet(Mediator& mediator, float deltaTime)
{
    for (const auto &bullet : Entities) {
        auto &script = mediator.GetComponent<Script>(bullet);
        auto &position = mediator.GetComponent<Position>(bullet);

        try {
            //script.executeFunction("update_bullet", deltaTime);
            // sol::table bullet_table = script.getState()["bullet"];
            // sol::table pos = bullet_table["position"];
            // position.x = pos["x"];
            // position.y = pos["y"];
        } catch (const sol::error &e) {
            std::cerr << "[Lua Error]: " << e.what() << " (Entity: " << bullet << ")" << std::endl;
        }
    }
}

std::unordered_set<Entity> BossSystem::Update_Boss(Mediator & _medi, std::unordered_set<Entity>& ent, float deltaTime)
{
    for (const auto &Boss : Entities) {
        auto &script = _medi.GetComponent<Script>(Boss);
        auto &position = _medi.GetComponent<Position>(Boss);
        auto &health = _medi.GetComponent<Health>(Boss);

        if (!script.luaScript) {
            std::cerr << "Lua script not initialized for boss: " << Boss << std::endl;
            continue;
        }

        try {
            // Appeler la fonction Lua "update_boss" pour mettre à jour le boss et récupérer les balles
            sol::object result = script.luaScript->executeFunction("update_boss", deltaTime);

            if (health.life >= 0) {
                script.executeFunction("set_health", health.life);
            }
            if (health.life <= 0) {
                Boos_dead = true;
            }
            // Vérifier si le résultat est une table
            if (result.is<sol::table>()) {
                sol::table bullets = result.as<sol::table>();

                // Vérifier que la table des balles est valide
                if (bullets.valid()) {
                    for (auto& bulletPair : bullets) {
                        sol::table bullet = bulletPair.second.as<sol::table>();

                        float bulletX = bullet["x"];
                        float bulletY = bullet["y"];
                        std::string bulletScript = bullet["script"];
                        int bulletType = bullet["type"];
                        int bulletDamage = bullet["damage"];
                        int collisionX = bullet["collisionx"];
                        int collisionY = bullet["collisiony"];
                        int angle = bullet["angle"];
                        int speed = bullet["speed"];
                    
                        Bullet newBullet = {bulletX, bulletY, bulletType, bulletDamage, bulletScript, collisionX, collisionY, angle, speed};
                        _bullets.push_back(newBullet);

                        //std::cout << "Spawned bullet at (" << bulletX << ", " << bulletY << ") with damage " << bulletDamage << std::endl;
                    }
                } else {
                    std::cerr << "Bullet table is invalid or not found." << std::endl;
                }
            } else {
                std::cerr << "update_boss did not return a table of bullets." << std::endl;
            }

            // Récupérer et mettre à jour la position du boss depuis Lua
            sol::optional<sol::table> bossOpt = script.getState()["boss"];
            if (bossOpt) {
                sol::table boss = *bossOpt;
                position.x = boss["position"]["x"];
                position.y = boss["position"]["y"];

                boss["position"]["x"] = position.x;
                boss["position"]["y"] = position.y;
            }

            //std::cout << "Enemy sytstem Boss position x = " << position.x << "y = " <<  position.y << "\n";
            // Vérifier si le boss a été vaincu
            if (health.life <= 0) {
                std::cout << "Boss " << Boss << " defeated.\n";
                ent.insert(Boss);
            }
        } catch (const sol::error &e) {
            std::cerr << "[Lua Error] Boss: " << Boss
                      << " Operation failed with error: " << e.what() << std::endl;
        }
    }
    return ent;
}
