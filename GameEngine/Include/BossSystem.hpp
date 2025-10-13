/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** BossSystem
*/

#ifndef BOSSSYSTEM_HPP_
    #define BOSSSYSTEM_HPP_
    #include "Entity.hpp"
    #include "Systeme.hpp"
    #include "Mediator.hpp"
    #include <unordered_map>
    class BossSystem : public System {
        public:
            void Init_Boss(Entity boss, Mediator & _medi, int posx, int posy, int health);
            void Update_Boss_bullet(Mediator & _medi, float deltaTime);
            std::unordered_set<Entity> Update_Boss(Mediator & _medi, std::unordered_set<Entity>& ent, float deltaTime);
            std::vector<Bullet>& getBullets(void) {return _bullets;}
            bool GetBossState(void) {return Boos_dead;}
            void SetBoosState(bool state) {Boos_dead = state;}
        protected:
        private:
            std::vector<Bullet> _bullets;
            float current_time = 0.0f;
            bool Boos_dead = false;
    };

#endif /* !BOSSSYSTEM_HPP_ */
