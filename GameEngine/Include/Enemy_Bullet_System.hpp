/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Enemy_Bullet_System
*/

#ifndef ENEMY_BULLET_SYSTEM_HPP_
    #define ENEMY_BULLET_SYSTEM_HPP_
    #include "Entity.hpp"
    #include "Systeme.hpp"
    #include "Mediator.hpp"

    class Enemy_Bullet_System : public System {
        public:
            std::unordered_set<Entity> Update_bullet(Mediator &mediator, float deltaTime, std::unordered_set<Entity>& entitiesToDelete);
            void init_bullet(Entity bullet, Mediator &mediator, float x, float y, int angle, int speed);
        protected:
        private:
    };

#endif /* !ENEMY_BULLET_SYSTEM_HPP_ */
