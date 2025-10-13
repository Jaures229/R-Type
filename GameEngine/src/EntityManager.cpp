#include "../Include/Entity.hpp"

EntityManager::EntityManager()
{
  Entity ent;
  for (ent = 0; ent < MAX_ENTITY; ++ent) {
    entitesFree.push(ent);
  }
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::CreateEntity () {
  assert (entitiesUsed < MAX_ENTITY && "Not enough place for another entity");

  Entity e = entitesFree.front();
  entitesFree.pop();
  ++entitiesUsed;
  return e;
}

void EntityManager::DestroyEntity(Entity entity) {
  assert(entity < MAX_ENTITY && "Out of range for entity");
  entitySignature[entity].reset();
  entitesFree.push(entity);
  --entitiesUsed;
}

void EntityManager::SetSignature(Entity entity, Signature signature) {
  assert(entity < MAX_ENTITY && "Out of range for entity");
  entitySignature[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) {
  assert(entity < MAX_ENTITY && "Out of range for entity");
  return(entitySignature[entity]);
}