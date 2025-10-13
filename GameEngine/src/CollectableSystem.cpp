/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** CollectableSystem
*/

#include "../Include/CollectableSystem.hpp"

std::unordered_set<Entity> CollectableSystem::Update(Mediator& mediator, std::unordered_set<Entity> &ent)
{
    for (auto const& entity : Entities) {
        auto& position = mediator.GetComponent<Position>(entity);
        //auto& power = mediator.GetComponent<Power>(entity);

        position.x -= 1;

        if (position.x <= 0) {
            ent.insert(entity);
        }
    }
    return ent;
}
