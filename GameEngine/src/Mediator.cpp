/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Mediator
*/

#include "../Include/Mediator.hpp"

void Mediator::Init() {
  componentManager = std::make_unique<ComponentManager>();
  entityManager = std::make_unique<EntityManager>();
  systemManager = std::make_unique<SystemManager>();
  
}

Entity Mediator::createEntity() {
  return entityManager->CreateEntity();
}

void Mediator::deleteEntity(Entity entity) {
  if (componentManager->HasComponent<Position>(entity)) {
    componentManager->DeleteComponent<Position>(entity);
  } else if (componentManager->HasComponent<AnimationComponent>(entity)) {
    componentManager->DeleteComponent<AnimationComponent>(entity);
  }  else if (componentManager->HasComponent<Script>(entity)) {
    componentManager->DeleteComponent<Script>(entity);
  } else if (componentManager->HasComponent<Sprite>(entity)) {
    componentManager->DeleteComponent<Sprite>(entity); 
  } else if (componentManager->HasComponent<Collision>(entity)) {
    componentManager->DeleteComponent<Collision>(entity);
  }
  
  entityManager->DestroyEntity(entity);
  componentManager->EntityDeleted(entity);
  systemManager->EntityDeleted(entity);
}
