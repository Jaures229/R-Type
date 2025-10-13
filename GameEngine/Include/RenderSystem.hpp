#ifndef RENDERSYS
  #define RENDERSYS
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"

  class RenderSystem : public System {
    public:
      void Update();
      void Render(sf::RenderWindow& window, Mediator& mediator);

    private:
  };

#endif
