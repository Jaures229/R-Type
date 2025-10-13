#ifndef COLLISIONSYS
  #define COLLISIONSYS
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"
  #include "Quadtree.hpp"
  
  class CollisionSystem : public System {
    public:
      bool checkCollision(const Collision& a, const Position& a1, const Collision& b, const Position& b1);
      std::unordered_set<Entity> updateCollision(Mediator& medi);
      int getdeadEnemy();
      std::vector<Collider> & getCollidedEnity(void) {return collide_entity;}
      int getBossState() { return boss_death;}
      void setBossState(int state) { boss_death = state;}
      void AdeadEnemy(int num);
      std::unordered_map<Entity, int>& getPlayersScore(void) {return _score_pair_players;}
    private:
      int Enemy;
      std::vector<Collider> collide_entity;
      int boss_death = 0;
      std::unordered_map<Entity, int> _score_pair_players;
  };
#endif
