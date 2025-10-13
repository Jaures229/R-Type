#include "../Include/SystemeManager.hpp"

void SystemManager::EntityDeleted(Entity entity) {
   for (auto const& a : Systems) {
    auto const& system = a.second;
    system->Entities.erase(entity);
   }
}

void SystemManager::EntitySignatureChanged (Entity entity, Signature entitySignature) {
  for (auto const& a : Systems) {
      auto const& type = a.first;
      auto const& system = a.second;
      auto const& sysSignature = Signatures[type];
      if ((entitySignature & sysSignature) == sysSignature) {
        system->Entities.insert(entity);
        //std::cout << "Entity added" << entity<< std::endl << std::endl;
      } else {
        system->Entities.erase(entity);
        //std::cout << "Entity deleted" << entity<< std::endl << std::endl;
      }
    }
}