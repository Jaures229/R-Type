#ifndef ENEMYSYS
  #define ENEMYSYS
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"

  class EnemySystem : public System {
    public:
      void Update();
      std::unordered_set<Entity> Movement(Mediator& mediator, float deltaTime);
      void Update_bullet(Mediator& mediator, float deltaTime);
      std::vector<Bullet> & getEnemyBullet(void) { return bullets;}
    private:
      std::vector<Bullet> bullets;
  };

#endif
