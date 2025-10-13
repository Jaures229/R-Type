#include "../Include/ComponentManager.hpp"


void ComponentManager::EntityDeleted (Entity entity) {
  for (auto const& a: ComponentArrays) {
    auto const& comp = a.second;
    comp->EntityDestroyed(entity);
  }
}
