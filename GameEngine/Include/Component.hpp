#ifndef COMPONENTS
  #define COMPONENTS

  #include "Entity.hpp"
  #include <string>
  #include "LuaScript.hpp"
  #include <unordered_map>

  struct Position
  {
    float x;
    float y;
  };
  
  struct Vitesse
  {
    float vx;
    float vy;
    float init_speed;
  };

  struct Bullet {
    float x;
    float y;
    int type;
    int damage;
    std::string script;
    int colx;
    int coly;
    int angle;
    int speed;
  };
  
  struct Health
  {
    int life;
  };
  struct Collider {
    int type;
    float x;
    float y;
  };

  struct Bullet_type {
    int type;
  };
  struct Damage
  {
    int damage;
  };

  struct Power {
    int health;
    int speed;
    int damage;
  };
  
  struct Sprite
  {
    std::string path;
    int width;
    int height;
    sf::Texture texture;
    sf::Sprite sprite;
  };
  
  struct Shoot
  {
    bool canShoot;
    float speed;
    Entity own;
    int type;
  };

  struct Boss {
    int life;
  };
  
  struct Collision
  {
    float x;
    float y;
    float width;
    float height;
    int type;
    bool isColliding;
  };

  struct Sound
  {
    enum SoundType {
      SHOOT,
      MAIN,
      LOOBY1,
      LOOBY2,
    };
    SoundType type;
    sf::SoundBuffer buff;
    sf::Sound sound;
  };

  struct Script {
    std::shared_ptr<LuaScript> luaScript; // Pointeur intelligent pour gérer LuaScript
    std::string scriptPath;

    Script() : scriptPath("") {}

    Script(const std::string& path)
        : luaScript(std::make_shared<LuaScript>()), scriptPath(path) {
        if (!luaScript->loadScript(scriptPath)) {
            std::cerr << "Failed to load script: " << scriptPath << std::endl;
        }
    }

    // Exécuter une fonction Lua avec des arguments optionnels
    template <typename... Args>
    void executeFunction(const std::string& funcName, Args... args) {
        luaScript->executeFunction(funcName, args...);
    }

    // Configurer une variable Lua
    template <typename T>
    void setVariable(const std::string& varName, const T& value) {
        luaScript->setVariable(varName, value);
    }
    // Accéder à l'état Lua
    sol::state& getState() {
        return luaScript->getState();
    }
};

  struct Animation {
    sf::IntRect startFrame;  // Position et taille de la première frame
    sf::Vector2i frameSize; // Largeur et hauteur des frames
    int frameCount;         // Nombre total de frames
    float frameDuration;    // Durée d'une frame
    bool looping;           // Si l'animation boucle

    sf::Vector2i frameOffset; // Décalage entre les frames (par défaut pas de décalage)
  };

  struct AI
  {
    float x;
    float initial_x;
  };

  struct AnimationComponent {
    std::unordered_map<std::string, Animation> animations; // Liste des animations disponibles
    std::string currentAnimation = "";                     // Animation active
    size_t currentFrame = 0;                               // Frame actuelle de l'animation active
    float elapsedTime = 0.0f;                              // Temps accumulé depuis la dernière frame
    bool active = true;                                    // Si une animation est active
  };
  
  class IComponentArray {
    public:
      virtual ~IComponentArray() = default;
      virtual void EntityDestroyed(Entity entity) = 0;
  };
  
  template <typename T>
  class ComponentArray : public IComponentArray {
    public:
      void PutData(Entity entity, T component) {
        assert(EntityToIndex.find(entity) == EntityToIndex.end() && "Component is already in the map.");
        size_t index = validEntry;
        EntityToIndex[entity] = index;
        IndexToEntity[index] = entity;
        arrayComponents[index] = component;
        ++validEntry;
      };

      void deleteData(Entity entity) {
        assert(EntityToIndex.find(entity) != EntityToIndex.end() && "Entity does't exit for deleting.");
        size_t indexToBeDeleted = EntityToIndex[entity];
        size_t indexOfLastElement = validEntry - 1;

        arrayComponents[indexToBeDeleted] = arrayComponents[indexOfLastElement];
        Entity entityOfThelastElement = IndexToEntity[indexOfLastElement];
        EntityToIndex[entityOfThelastElement] = indexToBeDeleted;
        IndexToEntity[indexToBeDeleted] = entityOfThelastElement;

        EntityToIndex.erase(entity);
        IndexToEntity.erase(indexOfLastElement);
        --validEntry;
      };

      T& GetData (Entity entity) {
        assert(EntityToIndex.find(entity) != EntityToIndex.end() && "Entity does't exit to get data.");

        return(arrayComponents[EntityToIndex[entity]]);
      };
      void EntityDestroyed(Entity entity) override {
        if(EntityToIndex.find(entity) != EntityToIndex.end()) {
          deleteData(entity);
        }
      };

      bool HasData (Entity entity)
      {
        return EntityToIndex.find(entity) != EntityToIndex.end();
      }

    private:
      std::array<T, MAX_ENTITY>arrayComponents;
      std::unordered_map<Entity, size_t>EntityToIndex;
      std::unordered_map<size_t, Entity>IndexToEntity;
      size_t validEntry;
  };

#endif
