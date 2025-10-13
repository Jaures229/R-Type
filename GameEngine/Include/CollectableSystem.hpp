/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** CollectableSystem
*/

#ifndef COLLECTABLESYSTEM_HPP_
    #define COLLECTABLESYSTEM_HPP_
    #include "Entity.hpp"
    #include "Systeme.hpp"
    #include "Mediator.hpp"

    class CollectableSystem : public System {
        public:
            std::unordered_set<Entity> Update(Mediator& mediator, std::unordered_set<Entity> &ent);
        protected:
        private:
    };

#endif /* !COLLECTABLESYSTEM_HPP_ */
