#ifndef PROJECTILESYS
  #define PROJECTILESYS
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"

  class ProjectileSystem : public System {
    public:
      void Update();
      void Movement(Mediator& mediator);
      std::unordered_set<Entity> check_out(Mediator& mediator, std::unordered_set<Entity>& entitiesToDelete);
    private:

  };

#endif