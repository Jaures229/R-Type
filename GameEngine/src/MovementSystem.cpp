#include "../Include/MovementSystem.hpp"

void MovementSystem::Update()
{

}

void MovementSystem::Movement(Mediator& mediator)
{
  for (auto const& entity : Entities) {
    auto& position = mediator.GetComponent<Position>(entity);
    auto& vitesse = mediator.GetComponent<Vitesse>(entity);
    position.x += vitesse.vx;
    position.y += vitesse.vy;
    //std::cout << "Entity " << entity << " pos to (" << position.x << ", " << position.y <<  "\n";

    vitesse.vx = 0;
    vitesse.vy = 0;

    //std::cout << "Entity " << entity << " moved to (" << position.x << ", " << position.y <<  "\n";
  }
}
