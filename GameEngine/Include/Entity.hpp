#ifndef ENTITY
    #define ENTITY

    #include <iostream>
    #include <cstdint>
    #include <string>
    #include <bitset>
    #include <queue>
    #include <array>
    #include <cassert>
    #include <unordered_map>
    #include <memory>
    #include <set>
    #include <typeindex>
    #include <SFML/Graphics.hpp>
    #include <SFML/Graphics/Texture.hpp>
    #include <SFML/Audio.hpp>
    #include  <unordered_set>

    using Entity = std::uint32_t;
    const Entity MAX_ENTITY = 1000;
    using Components = std::uint32_t;
    const Components MAX_COMPONENTS = 32;
    using Signature = std::bitset<MAX_COMPONENTS>;
    
    class EntityManager {
        public:
            EntityManager();
            ~EntityManager();
            Entity CreateEntity();
            void DestroyEntity(Entity entity);
            void SetSignature(Entity entity, Signature signature);
            Signature GetSignature(Entity entity);

        private:
            std::queue<Entity> entitesFree{};
            std::array<Signature, MAX_ENTITY> entitySignature{};
            std::uint32_t entitiesUsed{};
    };

#endif