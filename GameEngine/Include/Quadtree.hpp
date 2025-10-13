/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Quadtree
*/

#ifndef QUADTREE_HPP_
    #define QUADTREE_HPP_
    #include "SFML/Graphics.hpp"
    #include "SFML/System.hpp"
    #include "SFML/Window.hpp"
    #include "SFML/Audio.hpp"
    #include "SFML/Network.hpp"
    #include <iostream>
    #include <vector>
    #include <unordered_map>
    #include <unordered_set>
    #include <memory>
    #include <cmath>
    #include <ctime>
    #include <cstdlib>
    #include <string>
    #include <fstream>
    #include <sstream>
    #include <algorithm>
    #include <functional>
    #include <thread>
    #include <mutex>
    #include <chrono>
    #include <random>
    #include <cassert>
    #include <array>
    #include "Entity.hpp"

    class Quadtree {
        public:
            Quadtree(int level, const sf::FloatRect& bounds) : level(level), bounds(bounds) {}
            void clear();
            void insert(Entity entity, const sf::FloatRect& entityBounds);
            std::vector<Entity> retrieve(const sf::FloatRect& targetBounds);

        private:
            void subdivide();
            int getChildIndex(const sf::FloatRect& entityBounds) const;

            static const int maxEntities = 4; // Nombre maximum d'entités par nœud avant subdivision
            static const int maxLevels = 5;   // Profondeur maximale du quadtree

            int level;                        // Niveau actuel (profondeur)
            sf::FloatRect bounds;             // Limites du nœud
            std::vector<std::pair<Entity, sf::FloatRect>> entities; // Entités dans ce nœud
            std::array<std::unique_ptr<Quadtree>, 4> children;      // Enfants (quadrants)
    };
#endif /* !QUADTREE_HPP_ */
