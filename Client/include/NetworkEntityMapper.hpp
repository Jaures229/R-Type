/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** NetworkEntityMapper
*/

#ifndef NETWORKENTITYMAPPER_HPP_
    #define NETWORKENTITYMAPPER_HPP_
    #include <iostream>
    #include <ostream>
    #include <fstream>
    #include <SFML/Network.hpp>
    #include <SFML/Graphics.hpp>
    #include "../../Server/include/protocol.hpp"
    #include "../../GameEngine/Include/GameLogic.hpp"
    #include "../../GameEngine/Include/Mediator.hpp"
    #include "../../GameEngine/Include/RenderSystem.hpp"
    #include "AssetManager.hpp"
    #include "../../GameEngine/Include/SoundSystem.hpp"
    #include "../../GameEngine/Include/AnimationSystem.hpp"
    #include <zlib.h>
    #include <vector>
    #include <stdexcept>
    #include <cstring>
    #include <atomic>
    #include <queue>
    #include <mutex>
    #include <condition_variable>
    #include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
    #include "boost/filesystem/fstream.hpp"
    #include "SoundManager.hpp"

    class NetworkEntityMapper {
        private:
            std::unordered_map<Entity, Entity> serverToLocalMap;  // map server ID -> local ID
            std::unordered_map<Entity, Entity> localToServerMap;  // map local ID -> server ID

        public:
            void mapEntity(Entity serverEntityId, Entity localEntityId);
            Entity getLocalEntity(Entity serverEntityId);
            Entity getServerEntity(Entity localEntityId);
            void removeMapping(Entity localEntityId);
        };
#endif /* !NETWORKENTITYMAPPER_HPP_ */
