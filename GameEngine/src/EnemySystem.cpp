#include "../Include/EnemySystem.hpp"

void EnemySystem::Update() 
{

}

std::unordered_set<Entity> EnemySystem::Movement(Mediator& mediator, float deltaTime)
{
    std::unordered_set<Entity> entitiesToDelete;
    for (auto const& entity : Entities) {
        auto& script = mediator.GetComponent<Script>(entity);
        auto& position = mediator.GetComponent<Position>(entity);

        //position.x -= 10;
        // Créer une table Lua pour la position
        sol::table positionTable = script.getState().create_table();
        positionTable["x"] = position.x;
        positionTable["y"] = position.y;

        //position.x -= 10;

        // Passer la table Lua au script
        script.getState()["position"] = positionTable;

        // Appeler la fonction "update" dans Lua
        try {
            script.executeFunction("update", deltaTime);
        } catch (const sol::error& e) {
            std::cerr << "[Lua Error]: " << e.what() << std::endl;
            continue; // Ignore cette entité si le script échoue
        }

        // Récupérer la position mise à jour depuis Lua
        sol::table updatedPositionTable = script.getState()["position"];
        if (updatedPositionTable) {
            position.x = updatedPositionTable["x"];
            position.y = updatedPositionTable["y"];
        } else {
            std::cerr << "[Error]: Position table not updated by Lua script\n";
        }

        if (position.x <= 0 || position.x > 3000 || position.y >= 1080) {
            entitiesToDelete.insert(entity);
        }
    }
    return entitiesToDelete;
}

void EnemySystem::Update_bullet(Mediator& mediator, float deltaTime)
{
    for (auto const& entity : Entities) {
        auto& script = mediator.GetComponent<Script>(entity);
        auto& position = mediator.GetComponent<Position>(entity);

        try {
            //script.executeFunction
            sol::table newBullets = script.luaScript->executeFunction("update_bullet", deltaTime);
            
            for (size_t i = 1; i <= newBullets.size(); ++i) {
                sol::table bullet = newBullets[i];
                float bulletX = bullet["x"];
                float bulletY = bullet["y"];
                std::string bulletScript = bullet["script"];
                int bulletType = bullet["type"];
                int bulletDamage = bullet["damage"];
                int collisionX = bullet["collisionx"];
                int collisionY = bullet["collisiony"];
                int angle = bullet["angle"];
                int speed = bullet["speed"];

                //std::cout << "Création d'une balle : x=" << bulletX << ", y=" << bulletY << ", script=" << bulletScript << ", type=" << bulletType << ", damage=" << bulletDamage << ", collisionX=" << collisionX << ", collisionY=" << collisionY << "\n";w
                // Créer la balle dans le système C++
                Bullet newBullet = {bulletX, bulletY, bulletType, bulletDamage, bulletScript, collisionX, collisionY, angle, speed};
                bullets.push_back(newBullet); // Ajouter la balle à la liste des projectiles
            }
        } catch (const sol::error& e) {
            std::cerr << "[Lua Error in update_bullet]: " << e.what() << std::endl;
        }
    }
}
