/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** AnimationSystem
*/

#ifndef ANIMATIONSYSTEM_HPP_
    #define ANIMATIONSYSTEM_HPP_
    #include "Entity.hpp"
    #include "Systeme.hpp"
    #include "Mediator.hpp"
    #include <SFML/Graphics.hpp>
    #include <unordered_map>

    class AnimationSystem : public System {
        public:
            void update(Mediator &medi, float deltaTime);
            void setAnimation(Mediator &medi, const std::string& name, Entity entity);
        protected:
        private:
    };

#endif /* !ANIMATIONSYSTEM_HPP_ */
