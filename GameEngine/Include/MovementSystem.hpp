#ifndef MOVEMENTSYSTEM
  #define MOVEMENTSYSTEM
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"

  class MovementSystem : public System {
    public:
      void Update();
      void Movement(Mediator& mediator);
    private:

  };

#endif