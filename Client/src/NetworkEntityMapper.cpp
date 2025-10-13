/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** NetworkEntityMapper
*/

#include "../include/NetworkEntityMapper.hpp"


void NetworkEntityMapper::mapEntity(Entity serverEntityId, Entity localEntityId)
{
    serverToLocalMap[serverEntityId] = localEntityId;
    localToServerMap[localEntityId] = serverEntityId;
}

Entity NetworkEntityMapper::getLocalEntity(Entity serverEntityId)
{
    auto it = serverToLocalMap.find(serverEntityId);
    return (it != serverToLocalMap.end()) ? it->second : Entity(-1);
}

Entity NetworkEntityMapper::getServerEntity(Entity localEntityId)
{
    auto it = localToServerMap.find(localEntityId);
    return (it != localToServerMap.end()) ? it->second : Entity(-1);
}

void NetworkEntityMapper::removeMapping(Entity localEntityId)
{
    Entity serverId = getServerEntity(localEntityId);
    if (serverId != Entity(-1)) {
        serverToLocalMap.erase(serverId);
        localToServerMap.erase(localEntityId);
    }
}
