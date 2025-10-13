/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** CollisionSystem
*/

#include "../Include/CollisionSystem.hpp"

// bool CollisionSystem::checkCollision(const Collision& a, const Position& a1, const Collision& b, const Position& b1) {
//   return (a1.x  <  b1.x + b.width && a1.x + a.width > b1.x && a1.y < b1.y + b.height && a1.y + a.height > b1.y);
// }

bool CollisionSystem::checkCollision(const Collision& colA, const Position& posA,
                    const Collision& colB, const Position& posB) {
    return !(posA.x + colA.width < posB.x ||  // Droite de A à gauche de B
             posA.x > posB.x + colB.width || // Gauche de A à droite de B
             posA.y + colA.height < posB.y || // Bas de A au-dessus de B
             posA.y > posB.y + colB.height);  // Haut de A en dessous de B
}

int CollisionSystem::getdeadEnemy()
{
  return Enemy;
}


void CollisionSystem::AdeadEnemy(int num)
{
    Enemy += num;
}

std::unordered_set<Entity> CollisionSystem::updateCollision(Mediator& medi) {
    std::unordered_set<Entity> entitiesToDelete;

    // Parcours des entités ayant le composant Collision
    for (auto it = Entities.begin(); it != Entities.end(); ++it) {
        auto entA = *it;
        auto& posA = medi.GetComponent<Position>(entA);
        auto& colA = medi.GetComponent<Collision>(entA);

        // Parcours des entités suivantes pour éviter les doublons
        for (auto it2 = std::next(it); it2 != Entities.end(); ++it2) {
            auto entB = *it2;
            auto& posB = medi.GetComponent<Position>(entB);
            auto& colB = medi.GetComponent<Collision>(entB);

            // Vérifier la collision entre entité A et B
            if (checkCollision(colA, posA, colB, posB)) {
                //std::cout << "is collison\n";
                // Gestion des collisions Projectile (type 1) <-> Ennemi (type 2)
                if (colA.type == 1 && colB.type == 2 || colA.type == 2 && colB.type == 1) {
                  
                    //std::cout << "collision bullet enemy \n";
                    Entity projectile = (colA.type == 1) ? entA : entB;
                    Entity enemy = (colA.type == 2) ? entA : entB;

                    auto& enemyHealth = medi.GetComponent<Health>(enemy);

                    //std::cout << "Enemy health  == " << enemyHealth.life << std::endl;                    
                    // Vérifier si l'ennemi a des vies
                    if (medi.HasComponent<Health>(enemy)) {
                      //std::cout << "renter dans has health\n";
                        auto& enemyHealth = medi.GetComponent<Health>(enemy);
                        auto& projectileDamage = medi.GetComponent<Damage>(projectile);

                        // Réduire les vies de l'ennemi
                        enemyHealth.life -= projectileDamage.damage;

                        // Vérifier si les vies de l'ennemi sont à zéro ou moins
                        if (enemyHealth.life <= 0) {
                            //std::cout << "enemy is dead\n";
                            Enemy++;
                            auto &projectile_shoot = medi.GetComponent<Shoot>(projectile);

                            _score_pair_players[projectile_shoot.own] += 10;
                            //_score_pair_players[projectile_shoot.own] += 10;
                            //std::cout << "score player " << projectile_shoot.own << " == " << _score_pair_players[projectile_shoot.own] << std::endl;

                            Collider col;
                            col.type = 1;
                            col.x = posB.x;
                            col.y = posB.y;
                            collide_entity.push_back(col);
                            entitiesToDelete.insert(enemy); // Marquer l'ennemi pour suppression
                        }
                        Collider col;
                        col.type = 0;
                        col.x = posB.x;
                        col.y = posB.y;
                        collide_entity.push_back(col);
                    }

                    // Marquer le projectile pour suppression après collision
                    entitiesToDelete.insert(projectile);
                } else if (colA.type == 0 && colB.type == 3 || colA.type == 3 && colB.type == 0) {
                    //std::cout << "collision player health \n";
                    Entity player = (colA.type == 0) ? entA : entB;
                    Entity powerup = (colA.type == 3) ? entA : entB;

                    auto &player_health = medi.GetComponent<Health>(player);

                    auto&powerup_type = medi.GetComponent<Power>(powerup);
    
                    player_health.life += powerup_type.health;
                    if (player_health.life >= 100) {
                        player_health.life = 100;
                    }
                    entitiesToDelete.insert(powerup);

                    //std::cout << "player life is " << player_health.life;

                } else if (colA.type == 0 && colB.type == 4 || colA.type == 4 && colB.type == 0) {
                    std::cout << "collision player damage \n";
                    Entity player = (colA.type == 0) ? entA : entB;
                    Entity powerup = (colA.type == 4) ? entA : entB;

                    auto &player_damage = medi.GetComponent<Damage>(player);
                    auto &powerup_type = medi.GetComponent<Power>(powerup);
                    auto &b_type = medi.GetComponent<Bullet_type>(player);

                   // Progression des types d'armes (0 à 5)
                    
                    player_damage.damage += powerup_type.damage;
                    if (player_damage.damage > 100) {
                        player_damage.damage = 100;
                    }
                    entitiesToDelete.insert(powerup);
                    if (b_type.type < 5) {  // Vérifie si on n'a pas atteint l'arme maximale
                        b_type.type++;  // Incrémente le type d'arme
                        std::cout << "Weapon upgraded to type " << b_type.type << "\n";
                    } else {
                        std::cout << "Maximum weapon level reached\n";
                    }
                } else if (colA.type == 0 && colB.type == 5 || colA.type == 5 && colB.type == 0) {
                    std::cout << "collision player speed \n";
                    Entity player = (colA.type == 0) ? entA : entB;
                    Entity powerup = (colA.type == 5) ? entA : entB;
                    
                    auto &player_speed = medi.GetComponent<Vitesse>(player);
                    auto &powerup_type = medi.GetComponent<Power>(powerup);
                    player_speed.init_speed += powerup_type.speed; 
                    entitiesToDelete.insert(powerup);

                } else if(colA.type == 0 && colB.type == 2 || colA.type == 2 && colB.type == 0) {
                    //std::cout << "collision player enemy \n";
                    Entity player = (colA.type == 0) ? entA : entB;
                    Entity enemy = (colA.type == 2) ? entA : entB;
                    auto &player_health = medi.GetComponent<Health>(player);
                    player_health.life -= 10;

                    if (player_health.life <= 0) {
                        player_health.life = 0;
                    }

                    Collider col;
                    col.type = -1;
                    col.x = posA.x;
                    col.y = posA.y;
                    collide_entity.push_back(col);

                    entitiesToDelete.insert(enemy);
                } else if (colA.type == 0 && colB.type == -1 || colA.type == -1 && colB.type == 0) {
                    //std::cout << "collision player bullet \n";
                    Entity player = (colA.type == 0) ? entA : entB;
                    Entity bullet = (colA.type == -1) ? entA : entB;

                    auto &player_health = medi.GetComponent<Health>(player);
                    player_health.life -= 10;

                    if (player_health.life <= 0) {
                        player_health.life = 0;
                    }

                    Collider col;
                    col.type = -1;
                    col.x = posA.x;
                    col.y = posA.y;
                    collide_entity.push_back(col);

                    entitiesToDelete.insert(bullet);
                } else if ((colA.type == 1 && colB.type == -5) || (colA.type == -5 && colB.type == 1)) {
                    // Identifier le boss et le projectile
                    Entity boss = (colA.type == -5) ? entA : entB;
                    Entity bullet = (colA.type == 1) ? entA : entB;

                    // Entity projectile = (colA.type == 1) ? entA : entB;
                    // Entity enemy = (colA.type == -5) ? entA : entB;

                    // Vérifier l'existence des composants
                    if (!medi.HasComponent<Health>(boss) || !medi.HasComponent<Damage>(bullet)) {
                        std::cerr << "Error: Missing Health or Damage component.\n";
                        //return;
                    }

                    // Récupérer les composants
                    auto &boss_health = medi.GetComponent<Health>(boss);
                    auto &projectileDamage = medi.GetComponent<Damage>(bullet);

                    // Réduire la vie du boss
                    boss_health.life -= projectileDamage.damage;

                    //std::cout << "Boss Health == " << boss_health.life << "\n";
                    // Vérifier si le boss est mort
                    if (boss_health.life <= 0) {
                        boss_health.life = 0;
                        boss_death = 1;

                        // Ajouter une collision spéciale pour indiquer la mort du boss
                        Collider col;
                        col.type = -2; // Type spécifique pour la mort du boss
                        col.x = posB.x;
                        col.y = posB.y;
                        collide_entity.push_back(col);

                        // Marquer le boss pour suppression
                        //entitiesToDelete.insert(boss);
                    }

                    // Ajouter une collision pour la balle et marquer pour suppression
                    Collider col;
                    col.type = 0; // Collision générique pour la balle
                    col.x = posB.x;
                    col.y = posB.y;
                    collide_entity.push_back(col);

                    entitiesToDelete.insert(bullet);
                }
            }
        }
    }

    return entitiesToDelete;
}
