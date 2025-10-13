#include "../Include/ProjectileSystem.hpp"


void ProjectileSystem::Update() 
{

}

void ProjectileSystem::Movement(Mediator& mediator)
{
   for (auto const& entity : Entities) {
    auto& position = mediator.GetComponent<Position>(entity);
    auto& shoot = mediator.GetComponent<Shoot>(entity);
    position.x += shoot.speed;
    //std::cout << "Entity " << entity << " pos to (" << position.x << ", " << position.y <<  "\n";
    //std::cout << "Entity " << entity << " moved to (" << position.x << ", " << position.y <<  "\n";
  }
}


std::unordered_set<Entity> ProjectileSystem::check_out(Mediator& mediator, std::unordered_set<Entity>& entitiesToDelete)
{
  for (auto const& entity : Entities) {
    auto& position = mediator.GetComponent<Position>(entity);
    if (position.x >= 1920 || position.x < 0) {
      entitiesToDelete.insert(entity);
    }
  }
  return entitiesToDelete;
}
