/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** ObstacleSystem
*/

#ifndef OBSTACLESYSTEM_HPP_
    #define OBSTACLESYSTEM_HPP_
    #include "Entity.hpp"
    #include "Systeme.hpp"
    #include "Mediator.hpp"

    class ObstacleSystem : public System {
        public:
            void Update(Mediator& mediator);
        protected:
        private:
    };

#endif /* !OBSTACLESYSTEM_HPP_ */
