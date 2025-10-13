#ifndef GAMELOGIC
  #define GAMELOGIC
  #include "Mediator.hpp"
  #include "sol/sol.hpp"
  #include "../../GameEngine/Include/Quadtree.hpp"
  #include "SFML/Graphics.hpp"

  class GameLogic {
    public:
      //GameLogic() = delete;
      GameLogic(Mediator& mediator);
      void InitGameLogic();
      void AddPlayer(int ID, int x, int y, int health, int damage,int speed);
      void RemovePlayer(int ID);
      void RemoveEnemy(int ID);
      void RemoveProjectile(int ID);
      void AddEnemy(int id, int x, int y);
      void AddNewEnemy(int uuid, const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health);
      void AddProjectile(int uuid, int x, int y, Entity own, int damage, int type);
      void AddEnemyBullet(int uuid, int x, int y, int damage, int type, std::string script, int colx, int coly);
      int getNbBullet();
      int getNbEnemy();

      void Delete_entity(std::unordered_set<Entity> entities);
      std::unordered_map<int, Entity>& getEnemyList();
      std::unordered_map<int, Entity>& GetEntityList(void) { return entities;}
      std::vector<std::pair<Entity, int>>& getPlayerBullet(void) { return player_bullet_type;}

      //void InputTreatement(int ID, int action);
      //void Update();
      //bool GameOver();

      Entity getEntity(int Id0);

      Entity getEnemy(int id);

      Entity getProjectile (int id);

      std::vector<int>& GetEnemyEntityList() { return enemy_entity;}
      std::vector<int>& GetProjectileList() { return projectile_entity;}
      std::vector<int>& GetEnemyBullet() {return enemy_bullet_entity;}
      int GetEnemyEntityListNb() {return enemy_entities.size();}

      void AddBullet(void) { nbBullet++;}

      int RemoveEntity(int ent);

      std::string GetProjectileId(int ent);

      std::string GetEnemeyId(int ent);

      void AddObstacle(int uuid, int x, int y, int type);

      void AddPowerUp(int uuid, int x, int y, std::string type, int colx, int coly, int health, int speed, int power);

      std::vector<int>& getObstaclesList();
      std::vector<int>& getBossList() {return Bosslist;}
      std::vector<int>& getpowerList() { return powerlist;}

      void AddBoss(int uuid, float x, float y, std::string type, int health, int colx, int coly, int nb_type, std::string script);
    private:
      std::unordered_map<int, Entity> playerlist;
      std::unordered_map<int, Entity> enemylist;
      std::unordered_set<Entity> enemy_entities;
      std::unordered_map<int, Entity> entities;
      std::vector<std::pair<Entity, int>> player_bullet_type;

      std::vector<int> enemy_entity;
      std::vector<int> projectile_entity;
      std::vector<int> obstacleslist;
      std::vector<int> Bosslist;
      std::vector<int> powerlist;
      std::vector<int> enemy_bullet_entity;

      std::unordered_map<int, Entity> projectilelist;
      
      int nbBullet;
      //Entity Boss;
      Mediator& medi;
      Signature signature;
  };

#endif
