/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Quadtree
*/

#include "../Include/Quadtree.hpp"

void Quadtree::clear()
{
    entities.clear();
    for (auto& child : children) {
        if (child) {
            child->clear();
            child.reset();
        }
    }
}

void Quadtree::insert(Entity entity, const sf::FloatRect& entityBounds)
{
    // Si ce nœud a des enfants, insérer l'entité dans le bon enfant
    if (!children.empty()) {
        int index = getChildIndex(entityBounds);
            if (index != -1) {
                children[index]->insert(entity, entityBounds);
                return;
            }
        }

        // Ajouter l'entité à ce nœud
        entities.emplace_back(entity, entityBounds);

        // Diviser si nécessaire
        if (entities.size() > maxEntities && level < maxLevels) {
            if (children.empty()) {
                subdivide();
            }

            auto it = entities.begin();
            while (it != entities.end()) {
                int index = getChildIndex(it->second);
                if (index != -1) {
                    children[index]->insert(it->first, it->second);
                    it = entities.erase(it);
                } else {
                    ++it;
                }
            }
        }
}

std::vector<Entity> Quadtree::retrieve(const sf::FloatRect& targetBounds) {
        std::vector<Entity> potentialCollisions;

        // Trouver les entités dans ce nœud
        for (const auto& pair : entities) {
            potentialCollisions.push_back(pair.first);
        }

        // Trouver les entités dans les enfants pertinents
        if (!children.empty()) {
            int index = getChildIndex(targetBounds);
            if (index != -1) {
                auto childCollisions = children[index]->retrieve(targetBounds);
                potentialCollisions.insert(
                    potentialCollisions.end(),
                    childCollisions.begin(),
                    childCollisions.end());
            } else {
                // Si l'objet chevauche plusieurs quadrants, vérifier tous les enfants
                for (const auto& child : children) {
                    auto childCollisions = child->retrieve(targetBounds);
                    potentialCollisions.insert(
                        potentialCollisions.end(),
                        childCollisions.begin(),
                        childCollisions.end());
                }
            }
        }

        return potentialCollisions;
}

void Quadtree::subdivide()
{
    float halfWidth = bounds.width / 2.0f;
    float halfHeight = bounds.height / 2.0f;
    float x = bounds.left;
    float y = bounds.top;

    children[0] = std::make_unique<Quadtree>(level + 1, sf::FloatRect(x, y, halfWidth, halfHeight));
    children[1] = std::make_unique<Quadtree>(level + 1, sf::FloatRect(x + halfWidth, y, halfWidth, halfHeight));
    children[2] = std::make_unique<Quadtree>(level + 1, sf::FloatRect(x, y + halfHeight, halfWidth, halfHeight));
    children[3] = std::make_unique<Quadtree>(level + 1, sf::FloatRect(x + halfWidth, y + halfHeight, halfWidth, halfHeight));
}

int Quadtree::getChildIndex(const sf::FloatRect& entityBounds) const
{
    int index = -1;
    float verticalMidpoint = bounds.left + bounds.width / 2.0f;
    float horizontalMidpoint = bounds.top + bounds.height / 2.0f;bool topQuadrant = entityBounds.top < horizontalMidpoint && entityBounds.top + entityBounds.height < horizontalMidpoint;

    bool bottomQuadrant = entityBounds.top > horizontalMidpoint;

    if (entityBounds.left < verticalMidpoint && entityBounds.left + entityBounds.width < verticalMidpoint) {
        if (topQuadrant) {
            index = 0;
        } else if (bottomQuadrant) {
            index = 2;
        }
    } else if (entityBounds.left > verticalMidpoint) {
        if (topQuadrant) {
                index = 1;
        } else if (bottomQuadrant) {
            index = 3;
        }
    }
    return index;   
}
