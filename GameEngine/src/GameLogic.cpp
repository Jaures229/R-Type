/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** GameLogic
*/

#include "../Include/GameLogic.hpp"

GameLogic::GameLogic(Mediator& mediator): medi(mediator)
{
    nbBullet = 0;
}

void GameLogic::InitGameLogic()
{
}

void GameLogic::AddPlayer(int ID, int x, int y, int health, int damage, int speed)
{
    Entity player = medi.createEntity();
    playerlist[ID] = player;

    std::cout << "Add player " << player << "\n";
    medi.AddComponent(player, Position{float(x), float(y)});
    medi.AddComponent(player, Vitesse{0, 0, float(speed)});
    medi.AddComponent(player, Collision{float(x), float(y), 64, 64, 0});
    medi.AddComponent(player, Damage{damage});
    medi.AddComponent(player, Health{health});
    medi.AddComponent(player, Bullet_type{0});

    //std::cout << "Player health == " << health;

    std::pair<Entity, int> bullet_type;
    bullet_type.first = player;
    bullet_type.second = 0;
    player_bullet_type.push_back(bullet_type);

    std::cout << "Game logic player " << ID << " position x: " << x << " y: " << y << "\n";
}

void GameLogic::RemovePlayer(int ID) {
  assert(playerlist.find(ID) == playerlist.end() && "Player not found");
  Entity player = playerlist[ID];
  medi.deleteEntity(player);
  playerlist.erase(ID);
}

Entity GameLogic::getEntity(int Id) {
  return playerlist[Id];
}

Entity GameLogic::getEnemy(int id)
{
  return enemylist[id];
}

Entity GameLogic::getProjectile(int id)
{
    auto it = projectilelist.find(id);
    if (it != projectilelist.end()) {
        return it->second;
    } else {
        return (-1);
    }
}

void GameLogic::AddEnemy(int id, int x, int y)
{
    Entity enemy = medi.createEntity();
    enemylist[id] = enemy;
    enemylist.size();
    medi.AddComponent(enemy, Position{float(x), float(y)});
    medi.AddComponent(enemy, AI{1, 0});
    medi.AddComponent(enemy, Collision{float(x), float(y), 100, 100, 2});

    //std::cout << "Game logic enemy position x: " << x << " y: " << y << "\n";
}

void GameLogic::AddProjectile(int uuid, int x, int y, Entity owner, int damage, int type)
{
    //Entity projectile = medi.createEntity();
    entities[uuid] = uuid;
  
    //projectilelist[nbBullet] = project;
    medi.AddComponent(entities[uuid], Position{float(x), float(y)});
    medi.AddComponent(entities[uuid], Shoot{true, 6, owner, type});
    medi.AddComponent(entities[uuid], Collision{float(x), float(y), 50, 50, 1});
    medi.AddComponent(entities[uuid], Damage{damage});

    //std::cout << "Owner " << owner << " shooted\n";
    //std::cout << "-------------------" << nbBullet << "---------------------------" << std::endl;
    projectile_entity.push_back(uuid);
}

void GameLogic::RemoveEnemy(int ID)
{
  assert(enemylist.find(ID) == enemylist.end() && "Remove enemy not found");
  //Entity player = enemylist[ID];
  enemylist.erase(ID);
  medi.deleteEntity(ID);
}

void GameLogic::RemoveProjectile(int ID) {
  assert(projectilelist.find(ID) == projectilelist.end() && "Player not found");
  Entity Project = projectilelist[ID];
  medi.deleteEntity(Project);
  projectilelist.erase(ID);
}

int GameLogic::getNbBullet()
{
  return nbBullet;
}

void GameLogic::AddNewEnemy(int uuid, const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health)
{
    //Entity enemy = medi.createEntity();
    entities[uuid] = uuid;
    medi.AddComponent(entities[uuid], Position{x, y});
    medi.AddComponent(entities[uuid], AI{1, 0});
    medi.AddComponent(entities[uuid], Collision{x, y, float(colx), float(coly), 2});
    medi.AddComponent(entities[uuid], Script(script));
    medi.AddComponent(entities[uuid], Health{health});

    //std::cout << "Add Enemy with health = " << health << "\n";
    // sf::FloatRect entityBounds(x, y, colx, coly);
    // _quadtree.insert(entities[uuid], entityBounds);
    enemy_entity.push_back(uuid);
}

void GameLogic::AddPowerUp(int uuid, int x, int y, std::string type, int colx, int coly, int health, int speed, int power)
{
    entities[uuid] = uuid;
    medi.AddComponent(entities[uuid], Position{float(x), float(y)});

    if (type == "Health") {
        medi.AddComponent(entities[uuid], Collision{float(x), float(y), float(colx), float(coly), 3});
        medi.AddComponent(entities[uuid], Power{health, speed, power});
    } else if (type == "Damage") {
        medi.AddComponent(entities[uuid], Collision{float(x), float(y), float(colx), float(coly), 4});   
        medi.AddComponent(entities[uuid], Power{health, speed, power});
    } else if (type == "Speed") {
        medi.AddComponent(entities[uuid], Collision{float(x), float(y), float(colx), float(coly), 5});
        medi.AddComponent(entities[uuid], Power{health, speed, power});
    }
    powerlist.push_back(uuid);
}


void GameLogic::AddEnemyBullet(int uuid, int x, int y, int damage, int type, std::string script, int colx, int coly)
{
    entities[uuid] = uuid;

    medi.AddComponent(entities[uuid], Position{float(x), float(y)});
    medi.AddComponent(entities[uuid], Script(script));
    medi.AddComponent(entities[uuid], Collision{float(x), float(y), float(colx), float(coly), -1});
    medi.AddComponent(entities[uuid], Damage{damage});
    medi.AddComponent(entities[uuid], Bullet_type{type});
    
    enemy_bullet_entity.push_back(uuid);
}

int GameLogic::getNbEnemy()
{
    return enemylist.size();
}

std::unordered_map<int, Entity>& GameLogic::getEnemyList()
{
    return enemylist;
}

void GameLogic::AddObstacle(int uuid, int x, int y, int type)
{
    //Entity obstacle = medi.createEntity();
    entities[uuid] = uuid;
    medi.AddComponent(entities[uuid], Position{float(x), float(y)});
    medi.AddComponent(entities[uuid], Collision{float(x), float(y), 100, 100, type});
    obstacleslist.push_back(uuid);
}

std::vector<int>& GameLogic::getObstaclesList()
{
    return obstacleslist;
}

int GameLogic::RemoveEntity(int ent)
{
    int rvalue = -1;
    return rvalue;
}


std::string GameLogic::GetEnemeyId(int ent)
{
    return "";
}

std::string GameLogic::GetProjectileId(int ent)
{
    return "";
}


void GameLogic::Delete_entity(std::unordered_set<Entity> ent)
{
}


void GameLogic::AddBoss(int uuid, float x, float y, std::string type, int health, int colx, int coly, int nb_type, std::string script)
{
    entities[uuid] = uuid;
    medi.AddComponent(entities[uuid], Position{x, y});
    medi.AddComponent(entities[uuid], Script{script});
    // collison -5 for th id of the boss
    medi.AddComponent(entities[uuid], Collision{x, y, float(colx), float(coly), -5});
    medi.AddComponent(entities[uuid], Health{health});
    medi.AddComponent(entities[uuid], Boss{nb_type});
    Bosslist.push_back(uuid);
}
