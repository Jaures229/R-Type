/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** EnemyBulletSystem
*/

#include "../Include/Enemy_Bullet_System.hpp"

std::unordered_set<Entity> Enemy_Bullet_System::Update_bullet(Mediator &mediator, float deltaTime, std::unordered_set<Entity>& entitiesToDelete)
{
    for (const auto &bullet : Entities) {
        auto &script = mediator.GetComponent<Script>(bullet);
        auto &position = mediator.GetComponent<Position>(bullet);

        try {

            sol::table positionTable = script.getState().create_table();
            positionTable["x"] = position.x;
            positionTable["y"] = position.y;
            // Charger et synchroniser les données avec Lua
            script.getState()["position"] = positionTable;

            // Appeler la fonction "update_bullet"
            script.executeFunction("update_bullet", deltaTime);

            // Récupérer la position mise à jour depuis Lua
            sol::table updatedPositionTable = script.getState()["position"];
            if (updatedPositionTable && updatedPositionTable["x"].valid() && updatedPositionTable["y"].valid()) {
                position.x = updatedPositionTable["x"];
                position.y = updatedPositionTable["y"];
            } else {
                std::cerr << "[Lua Error]: Invalid position table for entity " << bullet << std::endl;
            }
        } catch (const sol::error &e) {
            std::cerr << "[Lua Error]: " << e.what() << " (Entity: " << bullet << ")" << std::endl;
        }

        // Supprimer les entités hors écran
        if (position.x < 0 || position.x > 1920 || position.y < 0 || position.y > 1080) {
            entitiesToDelete.insert(bullet);
        }
    }
    return entitiesToDelete;
}

void Enemy_Bullet_System::init_bullet(Entity bullet, Mediator &mediator, float x, float y, int angle, int speed)
{
    auto &script = mediator.GetComponent<Script>(bullet);

    try {
        script.executeFunction("init_bullet", x, y, angle, speed);
    } catch(const sol::error &e) {
        std::cerr << "[Lua Error]: " << e.what() << " (Entity: " << bullet << ")" << std::endl;
    }
}
