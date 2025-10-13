#ifndef SYSTEME
    #define SYSTEME
    #include "Entity.hpp"

    class System {
      public:
        std::set<Entity> Entities{};
    };
  #endif