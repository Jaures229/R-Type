#ifndef MEDIATOR
    #define MEDIATOR
    #include "Entity.hpp"
    #include "ComponentManager.hpp"
    #include "SystemeManager.hpp"

class Mediator {
  public:
    void Init();
    Entity createEntity();
    void deleteEntity(Entity entity);
    template <typename T>
    void RegisterComponent() {
      componentManager->registerComponent<T>();
    };

    template <typename T>
    void AddComponent(Entity entity, T component) {
      componentManager->AddComponent<T>(entity, component);
      auto signature = entityManager->GetSignature(entity);
      signature.set(componentManager->GetComponentType<T>(), true);
      entityManager->SetSignature(entity, signature);
      systemManager->EntitySignatureChanged(entity, signature);
    };

    template <typename T>
    bool HasComponent(Entity entity) {
      return componentManager->HasComponent<T>(entity);
    }

    template <typename T>
    void DeleteComponent (Entity entity) {
      componentManager->DeleteComponent<T>(entity);
      auto signature = entityManager->GetSignature(entity);
      signature.set(componentManager->GetComponentType<T>(), false);
      entityManager->SetSignature(entity, signature);
      systemManager->EntitySignatureChanged(entity, signature);
    };

    template <typename T>
    T& GetComponent(Entity entity) {
      return componentManager->GetComponent<T>(entity);
    };

    template <typename T>
    Components GetComponentType() {
      return componentManager->GetComponentType<T>();
    };

    template <typename T>
    std::shared_ptr<T> RegisterSystem() {
       return systemManager->RegisterSystem<T>();
    };

    template <typename T> 
    void SetSystemSignature (Signature signature) {
      systemManager->SetSignature<T>(signature);
    };

  private:
    std::unique_ptr<ComponentManager>componentManager;
    std::unique_ptr<EntityManager>entityManager;
    std::unique_ptr<SystemManager>systemManager;
};
#endif